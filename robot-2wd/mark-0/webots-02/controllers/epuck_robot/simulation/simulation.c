#include <stdlib.h> // Bibliotecas padrão C
#include <string.h>
#include <webots/robot.h> // Bibliotecas Webots

static int time_step = -1;        /// Time step da simulação em ms (cache lazy-initialized)
static int camera_time_step = -1; /// Time step da câmera em ms (e-puck: 1024, e-puck2: 64)

/// Obtém time step da simulação (lazy-initialized)
int simulation_get_time_step()
{
    if (time_step == -1)
        time_step = (int)wb_robot_get_basic_time_step();
    return time_step;
}

/// Obtém time step da câmera conforme modelo (e-puck2: 64ms, original: 1024ms)
int simulation_get_camera_time_step()
{
    if (camera_time_step == -1)
    {
        if (strcmp(wb_robot_get_model(), "GCtronic e-puck2") == 0)
        {
            camera_time_step = 64;
        }
        else
        { // original e-puck
            camera_time_step = 1024;
        }
    }
    return camera_time_step;
}

/// Executa um passo de simulação (sai automaticamente se interrompido)
void step()
{
    if (wb_robot_step(simulation_get_time_step()) == -1)
    {
        wb_robot_cleanup();
        exit(EXIT_SUCCESS);
    }
}

/// Aguarda por período especificado em segundos (mantém simulação ativa)
void passive_wait(double sec)
{
    double start_time = wb_robot_get_time();
    do
    {
        step();
    } while (start_time + sec > wb_robot_get_time());
}

/// Inicializa o robô e o sistema Webots
void simulation_init()
{
    wb_robot_init();
}

/// Limpa e libera recursos do robô
void simulation_cleanup(void)
{
    wb_robot_cleanup();
}