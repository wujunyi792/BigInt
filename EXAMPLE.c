# define MAX_BIT_LEN 1000

#include"number.h"

int main() {
    BigNum *op1, *op2, *rst;

    printf("////////////   ��ʾʾ��   ///////////////\n");

    while ((op1 = MakeNumFromIO()) == NULL);
//    op1 = MakeNum("10000000000.3");
    while ((op2 = MakeNumFromIO()) == NULL);
//    op2 = MakeNum("-66.79.3");


    printf("������ op1=");
    ShowNum(op1);
    printf("������ op2=");
    ShowNum(op2);
    printf("\n");

    printf("�����Ƚ� ");
    int res = Compare(op1, op2);
    switch (res) {
        case 1:
            printf("op1 > op2\n");
            break;
        case 0:
            printf("op1 = op2\n");
            break;
        case -1:
            printf("op1 < op2\n");
            break;
        default:
            printf("");
    }


    rst = BigNum_ADD(op1, op2);
    printf("�ӷ������");
    ShowNum(rst);
    rst = BigNum_SUB(op1, op2);
    printf("���������");
    ShowNum(rst);
    rst = BigNum_MUL(op1, op2);
    printf("�˷������");
    ShowNum(rst);
    rst = BigNum_DIV(op1, op2, 500); //tail ���С��λ��
    printf("���������");
    ShowNum(rst);

    printf("\n\n֧����������, ��ʾ op1 * op2 - 2 = ");
    rst = BigNum_SUB(BigNum_MUL(op1, op2), MakeNum("2"));
    ShowNum(rst);


    return 0;
}