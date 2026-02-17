// Bibliotecas padrão C
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
// Bibliotecas dependentes de plataforma
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#endif
// Bibliotecas Webots
#include <webots/device.h>
#include <webots/led.h>
#include <webots/motor.h>
#include <webots/robot.h>
#include <webots/speaker.h>
// Bibliotecas do projeto
#include "features.h"


#define MOTOR_RATIO 0.00628
#define LEDS_NUMBER 10

static WbDeviceTag leds[LEDS_NUMBER];
static WbDeviceTag left_motor;
static WbDeviceTag right_motor;
static WbDeviceTag speaker;

static bool leds_values[LEDS_NUMBER];

static const char *leds_names[LEDS_NUMBER] = {
    "led0", "led1", "led2", "led3", "led4",
    "led5", "led6", "led7", "led8", "led9"};

// Actuators
void actuators_init(void)
{
    left_motor = wb_robot_get_device("left wheel motor");
    right_motor = wb_robot_get_device("right wheel motor");
    wb_motor_set_position(left_motor, INFINITY);
    wb_motor_set_position(right_motor, INFINITY);
    wb_motor_set_velocity(left_motor, 0.0);
    wb_motor_set_velocity(right_motor, 0.0);
}

void actuators_update(void)
{
    // Update actuators
}

void reset_actuator_values(void)
{
    for (int i = 0; i < LEDS_NUMBER; i++)
        leds_set(i, 0);
}

void actuators_cleanup(void)
{
    // Cleanup actuators
}

// LEDs
void leds_init(void)
{
    int i;
    for (i = 0; i < LEDS_NUMBER; i++)
    {
        leds[i] = wb_robot_get_device(leds_names[i]);
        leds_values[i] = false;
    }
}

void leds_set(int led_id, int state)
{
    if (led_id >= 0 && led_id < LEDS_NUMBER)
    {
        leds_values[led_id] = (state != 0);
    }
}

void leds_update(void)
{
    int i;
    for (i = 0; i < LEDS_NUMBER; i++)
    {
        wb_led_set(leds[i], leds_values[i]);
    }
}

void leds_cleanup(void)
{
    // Cleanup LEDs
}

void blink_leds(void)
{
    static int counter = 0;
    int led = (counter / 10) % LEDS_NUMBER;
    for (int i = 0; i < LEDS_NUMBER; i++)
        leds_set(i, i == led ? 1 : 0);
    counter++;
}

void leds_set_by_accelerometer(double accel_x, double accel_y)
{
    // Define LEDs baseado na orientação do acelerômetro
    if (accel_x <= 0.0 && accel_y <= 0.0)
        leds_set(1, 1);
    if (accel_x <= 0.0 && accel_y > 0.0)
        leds_set(3, 1);
    if (accel_x > 0.0 && accel_y > 0.0)
        leds_set(5, 1);
    if (accel_x > 0.0 && accel_y <= 0.0)
        leds_set(7, 1);
}

void leds_clear_all(void)
{
    for (int i = 0; i < LEDS_NUMBER; i++)
        leds_set(i, 0);
}

// Audio
void audio_init(void)
{
    speaker = wb_robot_get_device("speaker");
}

void audio_play(void)
{
    // Play audio
}

void audio_stop(void)
{
    // Stop audio
}

void audio_cleanup(void)
{
    // Cleanup audio
}

// ============== API/Socket Communication Implementation ==============
static int server_fd = -1;
static int client_fd = 0;

#ifdef _WIN32
static bool socket_init()
{
    WSADATA info;
    if (WSAStartup(MAKEWORD(1, 1), &info) != 0)
    {
        fprintf(stderr, "Cannot initialize Winsock.\n");
        return false;
    }
    return true;
}
#endif

static bool socket_set_non_blocking(int fd)
{
    if (fd < 0)
        return false;
#ifdef _WIN32
    unsigned long mode = 1;
    return (ioctlsocket(fd, FIONBIO, &mode) == 0) ? true : false;
#else
    int flags = fcntl(fd, F_GETFL, 0) | O_NONBLOCK;
    return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
#endif
}

static int socket_accept(int server_fd)
{
    int cfd;
    struct sockaddr_in client;
    const struct hostent *client_info;
#ifndef _WIN32
    socklen_t asize;
#else
    int asize;
#endif
    asize = sizeof(struct sockaddr_in);
    cfd = accept(server_fd, (struct sockaddr *)&client, &asize);
    if (cfd == -1)
    {
#ifdef _WIN32
        int e = WSAGetLastError();
        if (e == WSAEWOULDBLOCK)
            return 0;
        fprintf(stderr, "Accept error: %d.\n", e);
#else
        if (errno == EWOULDBLOCK)
            return 0;
        fprintf(stderr, "Accept error: %d.\n", errno);
#endif
        return -1;
    }
    client_info = gethostbyname((char *)inet_ntoa(client.sin_addr));
    printf("Accepted connection from: %s.\n", client_info->h_name);
    return cfd;
}

static bool socket_close(int fd)
{
#ifdef _WIN32
    return (closesocket(fd) == 0) ? true : false;
#else
    return (close(fd) == 0) ? true : false;
#endif
}

static bool socket_cleanup()
{
#ifdef _WIN32
    return (WSACleanup() == 0) ? true : false;
#else
    return true;
#endif
}

static int create_socket_server(int port)
{
    int sfd, rc;
    struct sockaddr_in address;
#ifdef _WIN32
    if (!socket_init())
        return -1;
#endif
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
    {
        fprintf(stderr, "Cannot create socket.\n");
        return -1;
    }
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons((unsigned short)port);
    address.sin_addr.s_addr = INADDR_ANY;
    rc = bind(sfd, (struct sockaddr *)&address, sizeof(struct sockaddr));
    if (rc == -1)
    {
        fprintf(stderr, "Cannot bind port %d.\n", port);
        socket_close(sfd);
        return -1;
    }
    if (listen(sfd, 1) == -1)
    {
        fprintf(stderr, "Cannot listen for connections.\n");
        socket_close(sfd);
        return -1;
    }
    return sfd;
}

/**
 * Inicializa o servidor de API na porta especificada
 * @param port Porta para ouvir conexões
 * @return file descriptor do servidor, -1 em caso de erro
 */
int api_init(int port)
{
    server_fd = create_socket_server(port);
    if (server_fd >= 0)
    {
        socket_set_non_blocking(server_fd);
        printf("Waiting for a connection on port %d...\n", port);
    }
    return server_fd;
}

/**
 * Aceita uma nova conexão de cliente
 * @return file descriptor do cliente, 0 se nenhuma conexão, -1 em erro
 */
int api_accept_connection(void)
{
    if (server_fd < 0)
        return -1;

    int cfd = socket_accept(server_fd);
    if (cfd > 0)
    {
        socket_set_non_blocking(cfd);
        client_fd = cfd;
    }
    return cfd;
}

/**
 * Processa comandos recebidos do cliente
 * @param fd file descriptor do cliente
 * @param command_buffer buffer com os 21 bytes do comando
 * @return número de bytes processados, 0 se conexão fechada, -1 em erro
 */
int api_process_command(int fd, unsigned char *command_buffer)
{
    if (!command_buffer)
        return -1;

    int n = recv(fd, (char *)command_buffer, 21, 0);

    if (n < 0)
    {
#ifdef _WIN32
        int e = WSAGetLastError();
        if (e == WSAECONNABORTED)
            fprintf(stderr, "Connection aborted.\n");
        else if (e == WSAECONNRESET)
            fprintf(stderr, "Connection reset.\n");
        else
            fprintf(stderr, "Error reading from socket: %d.\n", e);
#else
        if (errno)
            fprintf(stderr, "Error reading from socket: %d.\n", errno);
#endif
        return -1;
    }

    if (n == 21 && command_buffer[0] == 0x80)
    {
        // Processa comando de motor
        double left_speed = MOTOR_RATIO * (command_buffer[3] + ((char)command_buffer[4] << 8));
        double right_speed = MOTOR_RATIO * (command_buffer[5] + ((char)command_buffer[6] << 8));
        wb_motor_set_velocity(left_motor, left_speed);
        wb_motor_set_velocity(right_motor, right_speed);

        // Processa LEDs
        wb_led_set(leds[0], (command_buffer[7] & 0x01) ? 1 : 0);
        wb_led_set(leds[2], (command_buffer[7] & 0x02) ? 1 : 0);
        wb_led_set(leds[4], (command_buffer[7] & 0x04) ? 1 : 0);
        wb_led_set(leds[6], (command_buffer[7] & 0x08) ? 1 : 0);
        wb_led_set(leds[8], (command_buffer[7] & 0x10) ? 1 : 0);
        wb_led_set(leds[9], (command_buffer[7] & 0x20) ? 1 : 0);
        wb_led_set(leds[1], ((int)command_buffer[8] << 16) + ((int)command_buffer[9] << 8) + command_buffer[10]);
        wb_led_set(leds[3], ((int)command_buffer[11] << 16) + ((int)command_buffer[12] << 8) + command_buffer[13]);
        wb_led_set(leds[5], ((int)command_buffer[14] << 16) + ((int)command_buffer[15] << 8) + command_buffer[16]);
        wb_led_set(leds[7], ((int)command_buffer[17] << 16) + ((int)command_buffer[18] << 8) + command_buffer[19]);
    }
    else if (n == 0)
    {
        wb_motor_set_velocity(left_motor, 0);
        wb_motor_set_velocity(right_motor, 0);
        printf("Connection closed, waiting for new connection...\n");
        socket_close(fd);
        client_fd = 0;
    }

    return n;
}

/**
 * Envia dados de sensores para o cliente
 * @param fd file descriptor do cliente
 * @param sensors_buffer buffer com dados dos sensores
 * @return true se enviado com sucesso
 */
bool api_send_sensors(int fd, unsigned char *sensors_buffer)
{
    if (fd <= 0 || !sensors_buffer)
        return false;

    return (send(fd, (char *)sensors_buffer, 105, 0) > 0);
}

/**
 * Envia uma imagem capturada da câmera para o cliente
 * @param fd file descriptor do cliente
 * @param image_buffer buffer com dados da imagem
 * @return true se enviado com sucesso
 */
bool api_send_image(int fd, unsigned char *image_buffer)
{
    if (fd <= 0 || !image_buffer)
        return false;

    return (send(fd, (char *)image_buffer, 38401, 0) > 0);
}

/**
 * Limpa recursos do servidor de API
 */
void api_cleanup(void)
{
    if (client_fd > 0)
        socket_close(client_fd);
    if (server_fd >= 0)
        socket_close(server_fd);

    socket_cleanup();
}

// ============== Bluetooth Communication Implementation ==============

void bluetooth_init(void)
{
    // Initialize Bluetooth communication
}

void bluetooth_send(const char *data)
{
    // Send data via Bluetooth
}

void bluetooth_receive(void)
{
    // Receive data via Bluetooth
}

void bluetooth_cleanup(void)
{
    // Cleanup Bluetooth communication
}

// ============== Features Control Hub ==============

void features_init(void)
{
    actuators_init();
    leds_init();
    audio_init();
    bluetooth_init();
}

void features_update(void)
{
    actuators_update();
    leds_update();
    bluetooth_receive();
}

void features_cleanup(void)
{
    actuators_cleanup();
    leds_cleanup();
    audio_cleanup();
    bluetooth_cleanup();
    api_cleanup();
}
