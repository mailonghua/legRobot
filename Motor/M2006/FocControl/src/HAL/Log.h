/*
 * Name:    debug.h
 *
 * Purpose: general debug system
 *
 * Copyright (C) 2014 wowotech
 * Subject to the GNU Public License, version 2.
 *
 * Created By:         wowo<www.wowotech.net>
 * Created Date:       2014-01-23
 *
 * ChangeList:
 * Created in 2014-01-23 by wowo;
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_
#include "Config/Config.h"
#ifndef ENABLE_CONFIG_GRAPH_UART
/* 
 * debug control, you can switch on (delete 'x' suffix)
 * to enable log output and assert mechanism
 */
#define CONFIG_ENABLE_DEBUG
#else
/*
* Use for enable Graph ,But disable CONFIG_ENABLE_DEBUG previous
* Receive PC Software:https://github.com/peng-zhihui/SerialChart
*/
#define CONFIG_GRAPH_ENABLE
#endif
/* 
 * debug level,
 * if is DEBUG_LEVEL_DISABLE, no log is allowed output,
 * if is DEBUG_LEVEL_ERR, only ERR is allowed output,
 * if is DEBUG_LEVEL_INFO, ERR and INFO are allowed output,
 * if is DEBUG_LEVEL_DEBUG, all log are allowed output,
 */

enum debug_level {
    DEBUG_LEVEL_DISABLE = 0,
    DEBUG_LEVEL_ERR,
    DEBUG_LEVEL_INFO,
    DEBUG_LEVEL_DEBUG
};



/* it can be change to others, such as file operations */
#include <stdio.h>
#define UART_WRITE          Serial.write
#define UART_READ           Serial.read
#define PRINT               Serial.printf
#define PRINTLN             Serial.println
#define LOG_UART_AVAILABLE  Serial.available

#define VOFA_UART_WRITE     Serial1.write
#define VOFA_UART_READ      Serial1.read
#ifdef CONFIG_ENABLE_DEBUG
/* 
 * the macro to set debug level, you should call it 
 * once in the files you need use debug system
 */
#define DEBUG_SET_LEVEL(x)  static int debug = x

#define ASSERT()                                        \
do {                                                    \
    PRINT("ASSERT: %s %s %d",                           \
           __FILE__, __FUNCTION__, __LINE__);           \
    while (1);                                          \
} while (0)

#define ERR(...)                                        \
do {                                                    \
    if (debug >= DEBUG_LEVEL_ERR) {                     \
        PRINT("[Error]");                               \
        PRINT(__VA_ARGS__);                             \
    }                                                   \
} while (0)

#define INFO(...)                                       \
do {                                                    \
    if (debug >= DEBUG_LEVEL_INFO) {                    \
        PRINT("[INFO]");                                \
        PRINT(__VA_ARGS__);                             \
    }                                                   \
} while (0)
#define INFOLN(...)                                     \
do {                                                    \
    if (debug >= DEBUG_LEVEL_INFO) {                    \
        PRINT("[INFO]");                                \
        PRINTLN(__VA_ARGS__);                             \
    }                                                   \
} while (0)
#define DEBUG(...)                                      \
do {                                                    \
    if (debug >= DEBUG_LEVEL_DEBUG) {                   \
        PRINT(__VA_ARGS__);                             \
    }                                                   \
} while (0)
#define DRAW_LINE(...)
#define FIRE_SEND(buffer,size)
#define FIRE_READ() 0
#elif defined(CONFIG_GRAPH_ENABLE)/*使用上位机数据传输*/
    #define DEBUG_SET_LEVEL(x) 
    #define ASSERT()
    #define ERR(...)
    #define INFO(...)
    #define INFOLN(...)  
    #define DEBUG_SET_LEVEL(x) 
    #define DEBUG(...)
    /*按类型选择0和1分别代表什么上位机软件，见config.h*/
    #if (DEBUG_DISPLAY_UI_TYPE==0) 
        #define FIRE_SEND(buffer,size)
        #define FIRE_READ() 0
        #define VOFA_SEND(buffer,size)
        #define VOFA_READ() 0
        #define DRAW_LINE(...)                                  \
        do{                                                     \
            PRINT(__VA_ARGS__);                                 \
        }while (0)
    #elif (DEBUG_DISPLAY_UI_TYPE==1)/*野火调试助手*/
        #define DRAW_LINE(...)
        #define FIRE_SEND(buffer,size)    \
                UART_WRITE(buffer,size);
        #define FIRE_READ() \
                UART_READ()
        #define VOFA_SEND(buffer,size)
        #define VOFA_READ() 0
    #elif (DEBUG_DISPLAY_UI_TYPE==2)/*VOFA调试助手*/
        #undef  DEBUG_SET_LEVEL 
        #define DEBUG_SET_LEVEL(x)  static int debug = x
        #define DRAW_LINE(...)
            #define FIRE_SEND(buffer,size)
        #define FIRE_READ() 0
        #define VOFA_SEND(buffer,size)    \
                VOFA_UART_WRITE(buffer,size);
        #define VOFA_READ() \
                VOFA_UART_READ()
        #undef  ERR
        #undef  INFO
        #define ERR(...)                                        \
        do {                                                    \
            if (debug >= DEBUG_LEVEL_ERR) {                     \
                PRINT("[Error]");                               \
                PRINT(__VA_ARGS__);                             \
            }                                                   \
        } while (0)

        #define INFO(...)                                       \
        do {                                                    \
            if (debug >= DEBUG_LEVEL_INFO) {                    \
                PRINT("[INFO]");                                \
                PRINT(__VA_ARGS__);                             \
            }                                                   \
        } while (0)
        #undef INFOLN
        #define INFOLN(...)                                     \
        do {                                                    \
            if (debug >= DEBUG_LEVEL_INFO) {                    \
                PRINT("[INFO]");                                \
                PRINTLN(__VA_ARGS__);                             \
            }                                                   \
        } while (0)
    #else
        #define DRAW_LINE(...)
        #define FIRE_SEND(buffer,size)
        #define FIRE_READ() 0
        #define VOFA_SEND(buffer,size)
        #define VOFA_READ() 0
    #endif
#else   /* CONFIG_ENABLE_DEBUG  */

#define DEBUG_SET_LEVEL(x) 
#define ASSERT()
#define ERR(...)
#define INFO(...)
#define INFOLN(...)  
#define DEBUG(...)
#define DRAW_LINE(...)
#define FIRE_SEND(buffer,size)
#define FIRE_READ() 0
#define VOFA_SEND(buffer,size)
#define VOFA_READ() 0


#endif  /* CONFIG_ENABLE_DEBUG  */

#endif  /* _DEBUG_H_ */