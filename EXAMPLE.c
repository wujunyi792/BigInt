# define MAX_BIT_LEN 1000

#include"Bint.h"

int main() {
    BigNum *op1, *op2, *rst;

    printf("////////////   演示示例   ///////////////\n");

    while ((op1 = MakeNumFromIO()) == NULL);
//    op1 = MakeNum("100");
    while ((op2 = MakeNumFromIO()) == NULL);
//    op2 = MakeNum("10");

    printf("操作数 op1=");
    ShowNum(op1);
    printf("\n操作数 op2=");
    ShowNum(op2);
    printf("\n\n");


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

    printf("\n\n支持连环调用, 演示 op1 * op2 - 2\n");
    rst = BigNum_SUB(BigNum_MUL(op1, op2), MakeNum("2"));
    ShowNum(rst);


    return 0;
}