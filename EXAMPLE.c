#include"Bint.h"

/*************************************
 这里可以定义自己的最长位数
 #define MAX_BIT_LEN  XXX
 ************************************/

int main() {
    BigNum *op1, *op2, *rst;

//    while ((op1 = MakeNumFromIO()) == NULL);
//    while ((op2 = BintGet("第二个操作数")) == NULL);
    op1 = MakeNum("-123.2");
    op2 = MakeNum("124");

    rst = BigNum_ADD(op1, op2);
    printf("加法结果：");
    ShowNum(rst);
    rst = BigNum_SUB(op1, op2);
    printf("减法结果：");
    ShowNum(rst);
    rst = BigNum_MUL(op1, op2);
    printf("乘法结果：");
    ShowNum(rst);
    rst = BigNum_DIV(op1, op2, 40);
    printf("除法结果：");
    ShowNum(rst);

    return 0;
}