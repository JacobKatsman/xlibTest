/* ----------------------------------------------------------------------------------------  */
/*  path: {$ROOT}/:                                                                          */
/*  Chikilevskiy A.N / aka "JZKatsman" 2025(c) / call89269081096@gmail.com                   */
/*                                                                                           */
/*    This is work make with educational and personal goal, for good wishes for all          */
/*    sentient beings. You may use this script for any your purpose.                         */
/*    BUT:                                                                                   */
/*    * MUST NOT BE USED TO: DUAL-USE ITEMS,                                                 */  
/*    * DO NOT USE OF THIS SCRIPT AS A PARTS WEAPON SYSTEM OR FOR OTHER WEAPONS PRODUCTION   */
/*    * PROHIBITED FOR USES THIS BY COPYRIGHT ORGANIZATIONS AND FOR COMMERCIAL AI-USES       */              
/*    * IT'S STRICTLY FORBIDDEN TO REMOVE THIS DISCLAIMER                                    */
/*                                                                                           */
/*  обработка графических примитивов в библиотеке xlib                                       */
/* ----------------------------------------------------------------------------------------  */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <sys/random.h> // Для getentropy
#include <openssl/rand.h>
#include "include/custom.h"
#include <png.h>

// *
// * "очень быстрый вывод точек на экран"
// * немного рефакторинга 
// *
// запустим профайлер
// gprof shm_example gmon.out > profile_report.txt
// valgrind --leak-check=full --show-leak-kinds=all ./shm_example

// sudo snap install codechecker --classic
// CodeChecker server -w ./ws -v 8555 &
// CodeChecker store ./reports --name my-project --url http://localhost:8555/Default
// firefox http://localhost:8555/Default

// 1.codechecker analyze /home/useralex/twopoint/ 
 
// gcc -fanalyzer -c src/*.c

/* Запустите Cppcheck, указав имя файла или директории с кодом на C:

Code

    cppcheck --enable=all --inconclusive --template="{file}:{line}:{column}:{severity}:{message}" your_code.c

В этом примере cppcheck выполнит анализ со всеми включенными правилами, выведет сообщения в формате "файл:строка:столбец:уровень:сообщение", а также проверит все файлы в текущей директории. 

    Проанализируйте вывод Cppcheck и исправьте найденные ошибки. 

В заключение, для сбора статистики по коду на языке C рекомендуется использовать комбинацию статического и динамического анализа, а также инструменты для анализа покрытия кода, производительности и зависимостей. Выбор конкретных инструментов зависит от задач и требований проекта. 

    Технологии статического анализа кода PVS-Studio
    Jan 11, 2022 — PVS-Studio предоставляет статические анализаторы для языков C, C++, C# и Java на платформах Windows, Linux и macOS.
    PVS-Studio

Краткое руководство. Анализ кода для C/C++ | Microsoft Learn
Чтобы выполнить анализ кода в файле, выполните следующую команду: * В Обозреватель решений выберите имя файла. * В меню "Сборка"
Microsoft Learn
Статический анализ кода - Википедия
Инструменты статического анализа * BLAST. * Clang Static Analyzer (встроен в Clang) * Coverity. * PC-Lint. * lint и lock_lint (Su...
*/

/*  alt - .  переход по функции по ссылке  */
/* ctags -e -R . */


int main(int argc, char *argv[]) {
    Context p;
    ShmStructure s;
    createWindowCustom(&p);
    checkShm(&p);
    createShm(&p, &s);
    /* Выбираем события,  которые будет обрабатывать программа */
    XSelectInput (p.display, p.window, ExposureMask | KeyPressMask);
    XMapWindow (p.display, p.window);
    /* главный цикл */  
    mainLoop (&p, &s);
    XShmDetach(p.display, &s.shm);
    XDestroyImage(s.image);
    XCloseDisplay(p.display);
  return 0;
}

//  TODO
//  loging
//  profiling
//  emacs  binding


 
