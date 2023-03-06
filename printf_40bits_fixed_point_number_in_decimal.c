#include <stdint.h>
#include <stdio.h>
#include <math.h>


int main()
{
    uint64_t sfix40_in = 0xff89375212;
    uint8_t fraction_len = 0x27;
    uint64_t integer_part=0;
    uint64_t decimal_par=0;
    double decimal=0;
    int cnt;
    uint8_t signal=0; //默认为正

    /* 1. 判断正负 */
    if(sfix40_in & 0x8000000000) // 为负
    {
        sfix40_in = ((~sfix40_in)+0x1) & 0x7FFFFFFFFF;
        signal = 1;
    }
    else // 为正
    {
        signal=0;
    }
    /* 2. 取小数部分 */
    decimal_par = sfix40_in & ( (uint64_t)(pow(2, fraction_len)) - 1);
    printf("decimal_par:%lld\r\n", decimal_par);
    cnt = 1;
    while(cnt<=fraction_len)
    {
        if( decimal_par & (((uint64_t)0x1) <<(fraction_len-cnt)) )
        {
            printf("cnt:%d\r\n", cnt);
            decimal += pow(2, -1*cnt);
        }
        cnt++;
    }
    /* 3. 整数部分 */
    integer_part = (sfix40_in & 0x7fffffffff) >> fraction_len;
    if((signal == 1) && (integer_part==0) && (decimal_par==0)) // 有可能有问题
    {
        printf("aa\r\n");
        integer_part = integer_part | (0x1<<(40-fraction_len-1)) ;
    }
    printf("integer_part:0x%x\r\n", integer_part);

    /* 4. 打印 */
    if(signal == 1)
    {
        printf("data:-%d.%.35lf\r\n",integer_part, decimal);
    }
    else if(signal == 0)
        printf("data:+%d.%.35lf\r\n",integer_part, decimal);



    // /* 1. 取小数部分 并计算 */
    // decimal_par = sfix40_in & ( (uint64_t)(pow(2, fraction_len)) - 1);
    // printf("decimal_par:%lld\r\n", decimal_par);
    // if(sfix40_in & 0x8000000000)
    // {
    //     uint64_t decimal_par_tmp;
    //     decimal_par_tmp = decimal_par;
    //     decimal_par_tmp = (~(decimal_par_tmp-0x1)) & (0xFFFFFFFFFF>>(40-fraction_len));
    //     decimal_par = decimal_par_tmp;
    // }
    // cnt = 1;
    // while(cnt<=fraction_len)
    // {
    //     if( decimal_par & (((uint64_t)0x1) <<(fraction_len-cnt)) )
    //     {
    //         printf("cnt:%d\r\n", cnt);
    //         decimal += pow(2, -1*cnt);
    //     }
    //     cnt++;
    // }
    // // 2. 整数部分
    // integer_part = (sfix40_in & 0x7fffffffff) >> fraction_len;
    // if(sfix40_in & 0x8000000000)
    // {
    //     integer_part = (~(integer_part)) & (0xFFFFFFFFFF>>fraction_len+1) ;
    //     printf("integer_part:0x%x\r\n", integer_part);
    // }
    // // 3. 打印
    // if(sfix40_in & 0x8000000000)
    // {
    //     printf("data:-%d.%.15lf\r\n",integer_part, decimal);
    // }
    // else
    //     printf("data:+%d.%.15lf\r\n",integer_part, decimal);

    return 0;
}