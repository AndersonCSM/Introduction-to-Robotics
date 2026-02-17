/// Servidor TCP/IP para comunicação remota com o robô
#include <stdio.h> // Bibliotecas padrão C
// Bibliotecas do projeto
#include "features.h"   // módulo de recursos adicionais
#include "simulation.h" // módulo de simulação

int pattern2(void)
{
    // Inicializa servidor na porta padrão
    int server_fd = api_init(API_DEFAULT_PORT);

    if (server_fd < 0)
    {
        fprintf(stderr, "Failed to initialize API server on port %d\n", API_DEFAULT_PORT);
        return -1;
    }

    int client_fd = 0;

    unsigned char command_buffer[21];
    unsigned char sensors_buffer[105] = {0};
    sensors_buffer[0] = 0x02;

    unsigned char *image_buffer = (unsigned char *)malloc(38401);
    image_buffer[0] = 0x01;

    bool stream_image = false;
    bool stream_sensors = false;

    // Loop principal
    while (wb_robot_step(simulation_get_time_step()) != -1)
    {
        // Aceita nova conexão se não conectado
        if (client_fd == 0)
        {
            client_fd = api_accept_connection();
            if (client_fd < 0)
                break;
        }

        // Processa comandos
        if (client_fd > 0)
        {
            int n = api_process_command(client_fd, command_buffer);

            if (n < 0)
                break;

            if (n == 21 && command_buffer[0] == 0x80)
            {
                // Extrai flags de streaming
                stream_image = ((command_buffer[1] & 1) == 1);
                stream_sensors = ((command_buffer[1] & 2) == 2);
            }
            else if (n == 0)
            {
                client_fd = 0;
            }
        }

        // Envia sensores se solicitado
        if (stream_sensors && client_fd > 0)
        {
            api_send_sensors(client_fd, sensors_buffer);
        }

        // Envia imagem se solicitada
        if (stream_image && client_fd > 0)
        {
            api_send_image(client_fd, image_buffer);
        }

        fflush(stdout);
    }

    // Limpeza
    free(image_buffer);
    api_cleanup();

    return 0;
}