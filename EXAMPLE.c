#include"Bint.h"

/*************************************
 ������Զ����Լ����λ��
 #define MAX_BIT_LEN  XXX
 ************************************/

int main() {
    BigNum *op1, *op2, *rst;

//    while ((op1 = MakeNumFromIO()) == NULL);
//    while ((op2 = BintGet("�ڶ���������")) == NULL);
    op1 = MakeNum("-123.2");
    op2 = MakeNum("124");

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

    return 0;
}