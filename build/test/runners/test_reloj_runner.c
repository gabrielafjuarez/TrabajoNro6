/* AUTOGENERATED FILE. DO NOT EDIT. */

/*=======Automagically Detected Files To Include=====*/
#include "unity.h"

int GlobalExpectCount;
int GlobalVerifyOrder;
char* GlobalOrderError;

/*=======External Functions This Runner Calls=====*/
extern void setUp(void);
extern void tearDown(void);
extern void test_hora_inicial(void);
extern void test_configurar_hora_actual (void);
extern void test_pasa_un_segundo(void);
extern void test_pasan_diez_segundos(void);
extern void test_pasa_un_minuto(void);
extern void test_pasan_diez_minutos(void);
extern void test_configurar_y_obtener_alarma (void);
extern void test_configurar_y_deshabilitar_alarma (void);
extern void test_configurar_y_disparar_alarma (void);
extern void test_configurar_y_no_disparar_alarma (void);


/*=======Mock Management=====*/
static void CMock_Init(void)
{
  GlobalExpectCount = 0;
  GlobalVerifyOrder = 0;
  GlobalOrderError = NULL;
}
static void CMock_Verify(void)
{
}
static void CMock_Destroy(void)
{
}

/*=======Teardown (stub)=====*/
void tearDown(void) {}

/*=======Test Reset Options=====*/
void resetTest(void);
void resetTest(void)
{
  tearDown();
  CMock_Verify();
  CMock_Destroy();
  CMock_Init();
  setUp();
}
void verifyTest(void);
void verifyTest(void)
{
  CMock_Verify();
}

/*=======Test Runner Used To Run Each Test=====*/
static void run_test(UnityTestFunction func, const char* name, UNITY_LINE_TYPE line_num)
{
    Unity.CurrentTestName = name;
    Unity.CurrentTestLineNumber = line_num;
#ifdef UNITY_USE_COMMAND_LINE_ARGS
    if (!UnityTestMatches())
        return;
#endif
    Unity.NumberOfTests++;
    UNITY_CLR_DETAILS();
    UNITY_EXEC_TIME_START();
    CMock_Init();
    if (TEST_PROTECT())
    {
        setUp();
        func();
    }
    if (TEST_PROTECT())
    {
        tearDown();
        CMock_Verify();
    }
    CMock_Destroy();
    UNITY_EXEC_TIME_STOP();
    UnityConcludeTest();
}

/*=======MAIN=====*/
int main(void)
{
  UnityBegin("test_reloj.c");
  run_test(test_hora_inicial, "test_hora_inicial", 30);
  run_test(test_configurar_hora_actual , "test_configurar_hora_actual ", 42);
  run_test(test_pasa_un_segundo, "test_pasa_un_segundo", 50);
  run_test(test_pasan_diez_segundos, "test_pasan_diez_segundos", 60);
  run_test(test_pasa_un_minuto, "test_pasa_un_minuto", 71);
  run_test(test_pasan_diez_minutos, "test_pasan_diez_minutos", 81);
  run_test(test_configurar_y_obtener_alarma , "test_configurar_y_obtener_alarma ", 91);
  run_test(test_configurar_y_deshabilitar_alarma , "test_configurar_y_deshabilitar_alarma ", 101);
  run_test(test_configurar_y_disparar_alarma , "test_configurar_y_disparar_alarma ", 112);
  run_test(test_configurar_y_no_disparar_alarma , "test_configurar_y_no_disparar_alarma ", 130);

  return UnityEnd();
}
