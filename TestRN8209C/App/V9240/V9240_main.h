#ifndef _V9240_MAIN_H_
#define _V9240_MAIN_H_





typedef enum{
	V9240_MAIN_INIT = 0,
	V9240_MAIN_CALIBRATE,		//校准基准的时候，必须要将输入接地
	V9240_MAIN_RUN,
}EN_V9240_MAIN_STATE;

#define IF_V9240_MAIN_STATE_OK(State) if(State > V9240_MAIN_RUN) return


EN_V9240_MAIN_STATE Get_V9240_Main_State(void);
void Set_V9240_Main_State(EN_V9240_MAIN_STATE State);
void V9240_Handler(void);


#endif

