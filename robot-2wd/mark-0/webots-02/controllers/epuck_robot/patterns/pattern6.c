/// Controlador comportamental completo com obstrução
#include <stdio.h>
#include <webots/robot.h>
/* Module includes */
#include "../include/environmental.h"
#include "../include/features.h"
#include "../include/locomotion.h"
#include "../include/motion.h"
#include "../include/simulation.h"

// Global defines
#define TRUE 1
#define FALSE 0
#define NO_SIDE -1
#define LEFT 0
#define RIGHT 1
#define WHITE 0
#define BLACK 1
#define SIMULATION 0 // for wb_robot_get_mode() function
#define REALITY 2    // for wb_robot_get_mode() function

// 8 IR proximity sensors
#define NB_DIST_SENS 8
#define PS_RIGHT_00 0
#define PS_RIGHT_45 1
#define PS_RIGHT_90 2
#define PS_RIGHT_REAR 3
#define PS_LEFT_REAR 4
#define PS_LEFT_90 5
#define PS_LEFT_45 6
#define PS_LEFT_00 7
const int PS_OFFSET_SIMULATION[NB_DIST_SENS] = {300, 300, 300, 300, 300, 300, 300, 300};
const int PS_OFFSET_REALITY[NB_DIST_SENS] = {480, 170, 320, 500, 600, 680, 210, 640};
int ps_value[NB_DIST_SENS] = {0, 0, 0, 0, 0, 0, 0, 0};

// 3 IR ground color sensors
#define NB_GROUND_SENS 3
#define GS_WHITE 900
#define GS_LEFT 0
#define GS_CENTER 1
#define GS_RIGHT 2
unsigned short gs_value[NB_GROUND_SENS] = {0, 0, 0};

//------------------------------------------------------------------------------
//
//    BEHAVIORAL MODULES
//
//------------------------------------------------------------------------------

/// LFM - Segue linha preta
// Braitenberg-like para linha

int lfm_speed[2];

#define LFM_FORWARD_SPEED 200
#define LFM_K_GS_SPEED 0.4

void LineFollowingModule(void)
{
  int DeltaS = gs_value[GS_RIGHT] - gs_value[GS_LEFT];

  lfm_speed[LEFT] = LFM_FORWARD_SPEED - LFM_K_GS_SPEED * DeltaS;
  lfm_speed[RIGHT] = LFM_FORWARD_SPEED + LFM_K_GS_SPEED * DeltaS;
}

/// OAM - Evita obstáculos
// Detecta e desvia com pesos de sensor

int oam_active, oam_reset;
int oam_speed[2];
int oam_side = NO_SIDE;

#define OAM_OBST_THRESHOLD 100
#define OAM_FORWARD_SPEED 150
#define OAM_K_PS_90 0.2
#define OAM_K_PS_45 0.9
#define OAM_K_PS_00 1.2
#define OAM_K_MAX_DELTAS 600

void ObstacleAvoidanceModule(void)
{
  int max_ds_value, i;
  int Activation[] = {0, 0};

  // Module RESET
  if (oam_reset)
  {
    oam_active = FALSE;
    oam_side = NO_SIDE;
  }
  oam_reset = 0;

  // Determine the presence and the side of an obstacle
  max_ds_value = 0;
  for (i = PS_RIGHT_00; i <= PS_RIGHT_45; i++)
  {
    if (max_ds_value < ps_value[i])
      max_ds_value = ps_value[i];
    Activation[RIGHT] += ps_value[i];
  }
  for (i = PS_LEFT_45; i <= PS_LEFT_00; i++)
  {
    if (max_ds_value < ps_value[i])
      max_ds_value = ps_value[i];
    Activation[LEFT] += ps_value[i];
  }
  if (max_ds_value > OAM_OBST_THRESHOLD)
    oam_active = TRUE;

  if (oam_active && oam_side == NO_SIDE) // check for side of obstacle only when not already detected
  {
    if (Activation[RIGHT] > Activation[LEFT])
      oam_side = RIGHT;
    else
      oam_side = LEFT;
  }

  // Forward speed
  oam_speed[LEFT] = OAM_FORWARD_SPEED;
  oam_speed[RIGHT] = OAM_FORWARD_SPEED;

  // Go away from obstacle
  if (oam_active)
  {
    int DeltaS = 0;
    // The rotation of the robot is determined by the location and the side of the obstacle
    if (oam_side == LEFT)
    {
      //(((ps_value[PS_LEFT_90]-PS_OFFSET)<0)?0:(ps_value[PS_LEFT_90]-PS_OFFSET)));
      DeltaS -= (int)(OAM_K_PS_90 * ps_value[PS_LEFT_90]);
      //(((ps_value[PS_LEFT_45]-PS_OFFSET)<0)?0:(ps_value[PS_LEFT_45]-PS_OFFSET)));
      DeltaS -= (int)(OAM_K_PS_45 * ps_value[PS_LEFT_45]);
      //(((ps_value[PS_LEFT_00]-PS_OFFSET)<0)?0:(ps_value[PS_LEFT_00]-PS_OFFSET)));
      DeltaS -= (int)(OAM_K_PS_00 * ps_value[PS_LEFT_00]);
    }
    else
    { // oam_side == RIGHT
      //(((ps_value[PS_RIGHT_90]-PS_OFFSET)<0)?0:(ps_value[PS_RIGHT_90]-PS_OFFSET)));
      DeltaS += (int)(OAM_K_PS_90 * ps_value[PS_RIGHT_90]);
      //(((ps_value[PS_RIGHT_45]-PS_OFFSET)<0)?0:(ps_value[PS_RIGHT_45]-PS_OFFSET)));
      DeltaS += (int)(OAM_K_PS_45 * ps_value[PS_RIGHT_45]);
      //(((ps_value[PS_RIGHT_00]-PS_OFFSET)<0)?0:(ps_value[PS_RIGHT_00]-PS_OFFSET)));
      DeltaS += (int)(OAM_K_PS_00 * ps_value[PS_RIGHT_00]);
    }
    if (DeltaS > OAM_K_MAX_DELTAS)
      DeltaS = OAM_K_MAX_DELTAS;
    if (DeltaS < -OAM_K_MAX_DELTAS)
      DeltaS = -OAM_K_MAX_DELTAS;

    // Set speeds
    oam_speed[LEFT] -= DeltaS;
    oam_speed[RIGHT] += DeltaS;
  }
}

/// LLM - Saída da linha
// Monitora abandono de trilha

int llm_active = FALSE, llm_inibit_ofm_speed, llm_past_side = NO_SIDE;
int lem_reset;

#define LLM_THRESHOLD 800

void LineLeavingModule(int side)
{
  // Starting the module on a rising edge of "side"
  if (!llm_active && side != NO_SIDE && llm_past_side == NO_SIDE)
    llm_active = TRUE;

  // Updating the memory of the "side" state at the previous call
  llm_past_side = side;

  // Main loop
  if (llm_active)
  { // Simply waiting until the line is not detected anymore
    if (side == LEFT)
    {
      if ((gs_value[GS_CENTER] + gs_value[GS_LEFT]) / 2 > LLM_THRESHOLD)
      { // out of line
        llm_active = FALSE;
        // *** PUT YOUR CODE HERE ***
        llm_inibit_ofm_speed = FALSE;
        lem_reset = TRUE;
        // *** PUT YOUR CODE HERE ***
      }
      else
      { // still leaving the line
        // *** PUT YOUR CODE HERE ***
        llm_inibit_ofm_speed = TRUE;
        // *** PUT YOUR CODE HERE ***
      }
    }
    else
    { // side == RIGHT
      if ((gs_value[GS_CENTER] + gs_value[GS_RIGHT]) / 2 > LLM_THRESHOLD)
      { // out of line
        llm_active = FALSE;
        // *** PUT YOUR CODE HERE ***
        llm_inibit_ofm_speed = FALSE;
        lem_reset = TRUE;
        // *** PUT YOUR CODE HERE ***
      }
      else
      { // still leaving the line
        // *** PUT YOUR CODE HERE ***
        llm_inibit_ofm_speed = TRUE;
        // *** PUT YOUR CODE HERE ***
      }
    }
  }
}

/// OFM - Segue obstáculo
// Contorna obstáculos com tendência de direção

int ofm_active;
int ofm_speed[2];

#define OFM_DELTA_SPEED 150

void ObstacleFollowingModule(int side)
{
  if (side != NO_SIDE)
  {
    ofm_active = TRUE;
    if (side == LEFT)
    {
      ofm_speed[LEFT] = -OFM_DELTA_SPEED;
      ofm_speed[RIGHT] = OFM_DELTA_SPEED;
    }
    else
    {
      ofm_speed[LEFT] = OFM_DELTA_SPEED;
      ofm_speed[RIGHT] = -OFM_DELTA_SPEED;
    }
  }
  else
  { // side = NO_SIDE
    ofm_active = FALSE;
    ofm_speed[LEFT] = 0;
    ofm_speed[RIGHT] = 0;
  }
}

/// LEM - Entrada na linha
// Máquina de estados para reentrada na trilha

int lem_active;
int lem_speed[2];
int lem_state, lem_black_counter;
int cur_op_gs_value, prev_op_gs_value;

#define LEM_FORWARD_SPEED 100
#define LEM_K_GS_SPEED 0.5
#define LEM_THRESHOLD 500

#define LEM_STATE_STANDBY 0
#define LEM_STATE_LOOKING_FOR_LINE 1
#define LEM_STATE_LINE_DETECTED 2
#define LEM_STATE_ON_LINE 3

void LineEnteringModule(int side)
{
  int Side, OpSide, GS_Side, GS_OpSide;

  // Reset de módulo
  if (lem_reset)
    lem_state = LEM_STATE_LOOKING_FOR_LINE;
  lem_reset = FALSE;

  // Inicializa velocidades
  lem_speed[LEFT] = LEM_FORWARD_SPEED;
  lem_speed[RIGHT] = LEM_FORWARD_SPEED;
  if (side == LEFT)
  {
    Side = RIGHT; // entra pela direita
    OpSide = LEFT;
    GS_Side = GS_RIGHT;
    GS_OpSide = GS_LEFT;
  }
  else
  {
    Side = LEFT; // entra pela esquerda
    OpSide = RIGHT;
    GS_Side = GS_LEFT;
    GS_OpSide = GS_RIGHT;
  }

  // Máquina de estados
  switch (lem_state)
  {
  case LEM_STATE_STANDBY:
    lem_active = FALSE;
    break;
  case LEM_STATE_LOOKING_FOR_LINE:
    if (gs_value[GS_Side] < LEM_THRESHOLD)
    {
      lem_active = TRUE;
      // Velocidades iniciais
      lem_speed[OpSide] = LEM_FORWARD_SPEED;
      lem_speed[Side] = LEM_FORWARD_SPEED;
      lem_state = LEM_STATE_LINE_DETECTED;
      // Salva sensor oposto
      if (gs_value[GS_OpSide] < LEM_THRESHOLD)
      {
        cur_op_gs_value = BLACK;
        lem_black_counter = 1;
      }
      else
      {
        cur_op_gs_value = WHITE;
        lem_black_counter = 0;
      }
      prev_op_gs_value = cur_op_gs_value;
    }
    break;
  case LEM_STATE_LINE_DETECTED:
    // Monitora sensor oposto
    if (gs_value[GS_OpSide] < LEM_THRESHOLD)
    {
      cur_op_gs_value = BLACK;
      lem_black_counter++;
    }
    else
      cur_op_gs_value = WHITE;
    // Detecta borda BLACK→WHITE
    if (prev_op_gs_value == BLACK && cur_op_gs_value == WHITE)
    {
      lem_state = LEM_STATE_ON_LINE;
      lem_speed[OpSide] = 0;
      lem_speed[Side] = 0;
    }
    else
    {
      prev_op_gs_value = cur_op_gs_value;
      // Ajusta velocidades para entrar na linha
      lem_speed[OpSide] = LEM_FORWARD_SPEED + LEM_K_GS_SPEED * (GS_WHITE - gs_value[GS_Side]);
      lem_speed[Side] = LEM_FORWARD_SPEED - LEM_K_GS_SPEED * (GS_WHITE - gs_value[GS_Side]);
    }
    break;
  case LEM_STATE_ON_LINE:
    oam_reset = TRUE;
    lem_active = FALSE;
    lem_state = LEM_STATE_STANDBY;
    break;
  }
}

//------------------------------------------------------------------------------
//
//    CONTROLLER
//
//------------------------------------------------------------------------------

/// Loop principal com arquitetura de subsunção completa
////////////////////////////////////////////
// Main
int main()
{
  int ps_offset[NB_DIST_SENS] = {0, 0, 0, 0, 0, 0, 0, 0}, i, speed[2], Mode = 1, time_step;
  int oam_ofm_speed[2];
  double distance_sensors[NB_DIST_SENS];
  double ground_sensors[NB_GROUND_SENS];

  /* Inicializa Webots */
  wb_robot_init();

  /* Time step da simulação */
  time_step = simulation_get_time_step();

  /* Inicializa módulos */
  distance_init();
  ground_init();
  leds_init();
  locomotion_init();

  for (;;)
  {
    wb_robot_step(time_step);

    // Reset ao trocar modo SIMULATION↔REALITY
    if (Mode != wb_robot_get_mode())
    {
      oam_reset = TRUE;
      llm_active = FALSE;
      llm_past_side = NO_SIDE;
      ofm_active = FALSE;
      lem_active = FALSE;
      lem_state = LEM_STATE_STANDBY;
      Mode = wb_robot_get_mode();
      if (Mode == SIMULATION)
      {
        for (i = 0; i < NB_DIST_SENS; i++)
          ps_offset[i] = PS_OFFSET_SIMULATION[i];
        locomotion_set_velocity(0, 0);
        wb_robot_step(time_step);
        printf("\n\nSwitching to SIMULATION\n");
      }
      else if (Mode == REALITY)
      {
        for (i = 0; i < NB_DIST_SENS; i++)
          ps_offset[i] = PS_OFFSET_REALITY[i];
        locomotion_set_velocity(0, 0);
        wb_robot_step(time_step);
        printf("\n\nSwitching to REALITY\n");
      }
    }

    // Atualiza sensores
    distance_update();
    distance_read(distance_sensors);
    ground_update();
    ground_read(ground_sensors);

    // Aplica offset e compensa sensores
    for (i = 0; i < NB_DIST_SENS; i++)
      ps_value[i] = (((int)distance_sensors[i] - ps_offset[i]) < 0) ? 0 : ((int)distance_sensors[i] - ps_offset[i]);
    for (i = 0; i < NB_GROUND_SENS; i++)
      gs_value[i] = ground_sensors[i];

    // Inicializa velocidades
    speed[LEFT] = 0;
    speed[RIGHT] = 0;

    // *** ARQUITETURA DE SUBSUNÇÃO ***

    // LFM
    LineFollowingModule();

    speed[LEFT] = lfm_speed[LEFT];
    speed[RIGHT] = lfm_speed[RIGHT];

    // OAM
    ObstacleAvoidanceModule();

    // LLM
    LineLeavingModule(oam_side);

    // OFM
    ObstacleFollowingModule(oam_side);

    // Inibição A (LLM inibe OFM)
    if (llm_inibit_ofm_speed)
    {
      ofm_speed[LEFT] = 0;
      ofm_speed[RIGHT] = 0;
    }

    // Soma A (OAM + OFM)
    oam_ofm_speed[LEFT] = oam_speed[LEFT] + ofm_speed[LEFT];
    oam_ofm_speed[RIGHT] = oam_speed[RIGHT] + ofm_speed[RIGHT];

    // Supressão A (OAM/OFM suprimem LFM)
    if (oam_active || ofm_active)
    {
      speed[LEFT] = oam_ofm_speed[LEFT];
      speed[RIGHT] = oam_ofm_speed[RIGHT];
    }

    // LEM
    LineEnteringModule(oam_side);

    // Supressão B (LEM suprime todos)
    if (lem_active)
    {
      speed[LEFT] = lem_speed[LEFT];
      speed[RIGHT] = lem_speed[RIGHT];
    }

    // *** FIM ARQUITETURA DE SUBSUNÇÃO ***

    // Debug
    printf("OAM %d side %d   LLM %d inibitA %d   OFM %d   LEM %d state %d\n", oam_active, oam_side, llm_active,
           llm_inibit_ofm_speed, ofm_active, lem_active, lem_state);

    // Aplica velocidades (0.00628 converte para rad/s)
    locomotion_set_velocity(0.00628 * speed[LEFT], 0.00628 * speed[RIGHT]);
  }
  return 0;
}
