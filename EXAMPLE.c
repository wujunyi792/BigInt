# define MAX_BIT_LEN 1000

#include"Bint.h"

int main() {
    BigNum *op1, *op2, *rst;

    printf("////////////   ��ʾʾ��   ///////////////\n");

    while ((op1 = MakeNumFromIO()) == NULL);
//    op1 = MakeNum("100");
    while ((op2 = MakeNumFromIO()) == NULL);
//    op2 = MakeNum("10");

    printf("������ op1=");
    ShowNum(op1);
    printf("\n������ op2=");
    ShowNum(op2);
    printf("\n\n");


    rst = BigNum_ADD(op1, op2);
    printf("�ӷ������");
    ShowNum(rst);
    rst = BigNum_SUB(op1, op2);
    printf("���������");
    ShowNum(rst);
    rst = BigNum_MUL(op1, op2);
    printf("�˷������");
    ShowNum(rst);
    rst = BigNum_DIV(op1, op2, 40);
    printf("���������");
    ShowNum(rst);

    printf("\n\n֧����������, ��ʾ op1 * op2 - 2\n");
    rst = BigNum_SUB(BigNum_MUL(op1, op2), MakeNum("2"));
    ShowNum(rst);


    return 0;
}