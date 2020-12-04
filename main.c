#include <thread.h>
#include <msg.h>
#include <xtimer.h>
#include <board.h>
#include <periph/gpio.h>
#include <arm_math.h>
#include <stdio.h>
#include <lptimer.h>


int32_t distance(int32_t lat1, int32_t lon1, int32_t lat2, int32_t lon2)
{
	int32_t rad = 6372795; //Радиус Земли в метрах
	q31_t diff_lat_long[2]; //Создаём массив для хранения полвины разницы широт и долгот
    						//[0] - lat
    						//[1] - long
	diff_lat_long[0] = 298 * (lat2 - lat1);
    diff_lat_long[1] = 298 * (lon1 - lon2);
    arm_abs_q31(diff_lat_long, diff_lat_long, 2);

    //Переводим широты в q31
    q31_t llat1 = 596 * lat1;
    q31_t llat2 = 596 * lat2;

    // Находим синусы косинусы этих значений
    q31_t sdlat = arm_sin_q31(diff_lat_long[0]);
    q31_t sdlon = arm_sin_q31(diff_lat_long[1]);


    //Находим квадраты этих значений
    q31_t sdlat_2;
    q31_t sdlon_2;
    arm_mult_q31(&sdlat, &sdlat, &sdlat_2, 1);
    arm_mult_q31(&sdlon, &sdlon, &sdlon_2, 1);


    // Косинусы широт
    q31_t cl1 = arm_cos_q31 (llat1);
    q31_t cl2 = arm_cos_q31 (llat2);

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
    int64_t itog = 2 * rad * (q63_t)sum_sqrt / 2147483648;


    return itog;

    
}

int main(void) {

    //Координаты двух чисел
    int32_t llat1=0;
    int32_t llong1=0;
    int32_t llat2 = 0;
    int32_t llong2 = 0;
    // Введите координаты первой точки (долгота, широта)
    scanf("%ld %ld", &llat1, &llong1);
    // Введите координаты второй точки (долгота, широта)
    scanf(" %ld %ld", &llat2, &llong2);

    printf("The distance: %ld\n", distance(llat1, llong1, llat2, llong2));
    


}

