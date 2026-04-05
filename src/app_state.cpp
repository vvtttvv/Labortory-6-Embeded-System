#include "app_state.h"

AppState g_app_state = {
  25.0f,
  50.0f,
  25.0f,
  0.8f,
  1,
  0,
  0
};

SemaphoreHandle_t g_state_mutex = NULL;