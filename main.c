#include <thread.h>
#include <msg.h>
#include <xtimer.h>
#include <board.h>
#include <periph/gpio.h>
#include <arm_math.h>
#include <stdio.h>
#include <lptimer.h>


int main(void) {
    int32_t rad = 6372795; //Радиус Земли в метрах


    //Координаты двух чисел
    int32_t llat1 = 557539;
    int32_t llong1 = 376203;
    int32_t llat2 = 568596;
    int32_t llong2 = 359118; 

    // Растояние между широтами и долготами, спереводом в q31
    q31_t diff_lat_long[2]; //Создаём массив для хранения полвины разницы широт и долгот
    						//[0] - lat
    						//[1] - long
	diff_lat_long[0] = 298 * (llat2 - llat1);
    diff_lat_long[1] = 298 * (llong1 - llong2);
    arm_abs_q31(diff_lat_long, diff_lat_long, 2);
    printf("%ld %ld\n", diff_lat_long[0], diff_lat_long[1]);


    //Переводим широты в q31
    q31_t lat1 = 596 * llat1;
    q31_t lat2 = 596 * llat2;

    // arm_sub_q31(&lat2, &lat1, &dlat, 2);
    // arm_sub_q31(&long2, &long1, &dlon, 2);

    // Делим эту разницу на 2 (идём по формуле)
    // q31_t dlat_0_5;
    // q31_t dlon_0_5;
    // arm_mult_q31(&dlat, &delenie_na_2, &dlat_0_5, 2);
    // arm_mult_q31(&dlon, &delenie_na_2, &dlon_0_5, 2);

    // Находим синусы косинусы этих значений
    q31_t sdlat = arm_sin_q31(diff_lat_long[0]);
    q31_t sdlon = arm_sin_q31(diff_lat_long[1]);
    // q31_t cos_lat1 = arm_cos_q31(lat1);
    // q31_t cos_lat2 = arm_cos_q31(lat2);

    //Находим квадраты этих значений
    q31_t sdlat_2;
    q31_t sdlon_2;
    arm_mult_q31(&sdlat, &sdlat, &sdlat_2, 1);
    arm_mult_q31(&sdlon, &sdlon, &sdlon_2, 1);


    // //В радианах, судя по всему это не нужно
    // arm_mult_q31 (&llat1, &pi, &lat1, 2); 
    // arm_mult_q31 (&llat2, &pi, &lat2, 2);
    // arm_mult_q31 (&llong1, &pi, &long1, 2);
    // arm_mult_q31 (&llong2, &pi, &long2, 2);


    // Косинусы широт
    q31_t cl1 = arm_cos_q31 (lat1);
    q31_t cl2 = arm_cos_q31 (lat2);

    // Перемножаем косинусы широт
    q31_t mcl;
    arm_mult_q31(&cl1, &cl2, &mcl, 1);

    //Умножаем произведение косинусов на квадрат синуса долготы
    q31_t second_term;
    arm_mult_q31(&mcl, &sdlon_2, &second_term, 1);

    // Складываем с квадратом синуса разницы широты
    q31_t sum;
    arm_add_q31(&second_term, &sdlat_2, &sum, 1);

    // Находим квадратный корень
    q31_t sum_sqrt;
    arm_sqrt_q31(sum, &sum_sqrt);

    // Находим расстояние
    printf("%ld\n", sum_sqrt);
    int64_t itog = 2 * rad * (q63_t)sum_sqrt;
    int32_t s = (int64_t) itog >> 31;
    printf("%ld\n", s);
    int64_t distance = itog / 2147483648;
    int32_t rast = distance;

    printf("%ld\n", rast);


}

