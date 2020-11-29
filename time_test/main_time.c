#include <thread.h>
#include <msg.h>
#include <xtimer.h>
#include <board.h>
#include <periph/gpio.h>
#include <arm_math.h>
#include <stdio.h>
#include <lptimer.h>


int main(void) {
    // int32_t rad = 6372795; //Радиус Земли в метрах
    q31_t delenie_na_2 = 1073741824;


    //Координаты двух чисел
    volatile int32_t vlat1 = 1932735283; //77.1539 771539
    volatile int32_t vlong1 = 4724464;

    volatile int32_t vlat2 = 1524713390; //77.1804
    volatile int32_t vlong2 = 740445;

	lptimer_init();
    printf("init, lptimer_now = %lu\n", lptimer_now().ticks32);
    for (int i = 0; i < 1000; ++i)
    {
       int32_t llat1 =vlat1 +i;
       int32_t llong1 = vlong1-i;
       int32_t llat2 = vlat2-i;
       int32_t llong2 = vlong2+i;

    // q31_t lat1;
    // q31_t lat2;
    // q31_t long1;
    // q31_t long2;

    // q31_t pi = 37461659; //pi / 180 в системе q_31


    // Растояние между широтами и долготами
    q31_t dlat;
    q31_t dlon;
    arm_sub_q31(&llat2, &llat1, &dlat, 2);
    arm_sub_q31(&llong2, &llong1, &dlon, 2);

    // Делим эту разницу на 2 (идём по формуле)
    q31_t dlat_0_5;
    q31_t dlon_0_5;
    arm_mult_q31(&dlat, &delenie_na_2, &dlat_0_5, 2);
    arm_mult_q31(&dlon, &delenie_na_2, &dlon_0_5, 2);

    // Находим синусы этих значений
    q31_t sdlat = arm_sin_q31(dlat_0_5);
    q31_t sdlon = arm_sin_q31(dlon_0_5);

    //Находим квадраты этих значений
    q31_t sdlat_2;
    q31_t sdlon_2;
    arm_mult_q31(&sdlat, &sdlat, &sdlat_2, 2);
    arm_mult_q31(&sdlon, &sdlon, &sdlon_2, 2);


    // //В радианах, судя по всему это не нужно
    // arm_mult_q31 (&llat1, &pi, &lat1, 2); 
    // arm_mult_q31 (&llat2, &pi, &lat2, 2);
    // arm_mult_q31 (&llong1, &pi, &long1, 2);
    // arm_mult_q31 (&llong2, &pi, &long2, 2);


    // Косинусы широт
    q31_t cl1 = arm_cos_q31 (llat1);
    q31_t cl2 = arm_cos_q31 (llat2);

    // Перемножаем косинусы широт
    q31_t mcl;
    arm_mult_q31(&cl1, &cl2, &mcl, 2);

    //Умножаем произведение косинусов на квадрат синуса долготы
    q31_t second_term;
    arm_mult_q31(&mcl, &sdlon_2, &second_term, 2);

    // Складываем с квадратом синуса разницы широты
    q31_t sum;
    arm_add_q31(&second_term, &sdlon_2, &sum, 2);

    // Находим квадратный корень
    q31_t sum_sqrt;
    arm_sqrt_q31(sum, &sum_sqrt);

    printf("%ld\n", sum_sqrt);



    }
    printf("init, lptimer_now = %lu\n", lptimer_now().ticks32);


}

