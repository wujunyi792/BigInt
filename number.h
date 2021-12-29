# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <assert.h>

/**
 * num -> char ��ض���
 */
#define ASCII_NUM_9 57
#define ASCII_NUM_0 48
#define ASCII_NUM_POINTER 46  //С����
#define POS  1
#define NEG  0
#define  Fix(n) ((n)-ASCII_NUM_0)
#define NOT_NUM 0x88

/**
 * ������ַ�������֧��
 */
#ifndef MAX_BIT_LEN
#define MAX_BIT_LEN 500
#endif

/**
 * �����ṹ�嶨��
 */
typedef char bitDec;
typedef unsigned long long int VeryLongInt; //�ڲ�����ʱ���õ�������ÿ�ζ�дһ�� unsigned long long int
typedef struct numberStr {
    bitDec numer[MAX_BIT_LEN * 2 + 1];//ʮ��������, *2-1��ֹ������
    char sign;//����λ
    unsigned long long point; //С����λ��
    unsigned long long len; //����
    int offset; //�Ƿ�������
} BigNum;

void ShowNum(BigNum *BigNum);

BigNum *MakeNumFromIO();

BigNum *MakeNum(char str[]);

BigNum *BigNum_ADD(BigNum *op1, BigNum *op2);

BigNum *BigNum_SUB(BigNum *op1, BigNum *op2);

BigNum *BigNum_MUL(BigNum *op1, BigNum *op2);

BigNum *BigNum_DIV(BigNum *op1, BigNum *op2, VeryLongInt tail);

BigNum *BigNum_Unsigned_ADD(BigNum *op1, BigNum *op2);

BigNum *BigNum_Unsigned_SUB(BigNum *op1, BigNum *op2);

BigNum *Clear(BigNum *BigNum);

int Compare(BigNum *op1, BigNum *op2);


/**
 * Des: ���û���������һ�������ṹ
 * @Argu:�ַ��� [��ʾ��Ϣ] 
 * Returns:ָ��  �����ṹ
 */
BigNum *MakeNumFromIO() {
    //����Ļ��� 
    char input[MAX_BIT_LEN + 1];

    //�����ṹ����
    BigNum *ret = (BigNum *) malloc(sizeof(BigNum));
    ret->point = 0;

    //����
    printf("[%s] ������һ������(%dλ����)\n", "������", MAX_BIT_LEN);
    scanf("%s", input);

    //�α�  ���㳤��
    char *p = input;

    /*�����ж�*/
    switch (*p) {
        case '-':
            ret->sign = NEG;
            p++;
            break;
        case '+':
            ret->sign = POS;
            p++;
            break;
        default:
            ret->sign = POS;
    }
    /*���㳤�Ȳ���ֵ*/
    int len_cnt = 0;
    char begin_flag = 1;
    int havePointer = 0;
    while (*p != '\0') {
        if (*p > ASCII_NUM_9 || (*p < ASCII_NUM_0 && *p != ASCII_NUM_POINTER)) {
            printf(":::������� ������\n");
            free(ret);
            return NULL;
        } else if (*p == ASCII_NUM_0 && begin_flag) {
            p++;
            continue;
        } else {
            if (*p == '.') {
                havePointer = 1;
                p++;
                continue;
            }
            begin_flag = 0;
            ret->numer[len_cnt] = Fix(*p);
            if (havePointer)ret->point++;
            p++;
            len_cnt++;
        }
    }
    //ȫΪ0
    if (*p == '\0' && begin_flag == 1) {
        ret->numer[0] = 0;
        ret->len = len_cnt = 1;
    }

    //��lenλ
    ret->len = len_cnt;
    ret->offset = 0;

    //����ʣ���Ϊ0x88 -120
    memset(&(ret->numer[len_cnt]), NOT_NUM, MAX_BIT_LEN - len_cnt);

    return ret;
}

/**
 * Des: ���ַ�������һ�������ṹ
 * @Argu �ַ��� [Ҫ�������ַ���]
 * Returns:ָ��  �����ṹ
 */
BigNum *MakeNum(char str[]) {
    //����Ļ���
    char input[MAX_BIT_LEN + 1];

    //�����ṹ����
    BigNum *ret = (BigNum *) malloc(sizeof(BigNum));
    ret->point = 0;

    //�α�  ���㳤��
    char *p = str;

    /*�����ж�*/
    switch (*p) {
        case '-':
            ret->sign = NEG;
            p++;
            break;
        case '+':
            ret->sign = POS;
            p++;
            break;
        default:
            ret->sign = POS;
    }
    /*���㳤�Ȳ���ֵ*/
    int len_cnt = 0;
    char begin_flag = 1;
    int havePointer = 0;
    while (*p != '\0') {
        if (*p > ASCII_NUM_9 || (*p < ASCII_NUM_0 && *p != ASCII_NUM_POINTER)) {
            printf(":::err input\n");
            free(ret);
            return NULL;
        } else if (*p == ASCII_NUM_0 && begin_flag) {
            p++;
            continue;
        } else {
            if (*p == '.') {
                havePointer = 1;
                p++;
                continue;
            }
            begin_flag = 0;
            ret->numer[len_cnt] = Fix(*p);
            if (havePointer)ret->point++;
            p++;
            len_cnt++;
        }
    }
    //ȫΪ0
    if (*p == '\0' && begin_flag == 1) {
        ret->numer[0] = 0;
        ret->len = len_cnt = 1;
    }

    //��lenλ
    ret->len = len_cnt;
    ret->offset = 0;

    //����ʣ���Ϊ0x88 -120
    memset(&(ret->numer[len_cnt]), NOT_NUM, MAX_BIT_LEN - len_cnt);

    return ret;
}

/**
 * �������
 * @param BigNum Ҫ���������
 */
void ShowNum(BigNum *BigNum) {

    // ȥ��С����
    while (BigNum->point > 0 && BigNum->numer[BigNum->len - 1] == 0) {
        BigNum->numer[BigNum->len - 1] = NOT_NUM;
        BigNum->point--;
        BigNum->len--;
    }
    if (BigNum->len == 1 && BigNum->numer[0] == 0)
        BigNum->sign = POS;

    if (BigNum->sign == NEG) {
        printf("-");
    }
    long long i;
    int havePointer = BigNum->point != -1;
    if (BigNum->len <= BigNum->point) {
        printf("0.");
        for (int j = 0; j < BigNum->point - BigNum->len; ++j) {
            printf("0");
        }
        havePointer = 0;
    }

    for (i = 0; i < BigNum->len; i++) {
        if (i == BigNum->len - BigNum->point && havePointer) {
            printf(".");
            havePointer = 0;
            i--;
            continue;
        }
        printf("%d", BigNum->numer[i]);
    }

    printf("\n");
}

/**
 * �ײ��޷��żӷ�
 * @param op1 ����1
 * @param op2 ����2
 * @return ������
 */
BigNum *BigNum_Unsigned_ADD(BigNum *op1, BigNum *op2) {
    unsigned long long maxLen, rstLen, gap;
    BigNum *rst, *L, *S;
    bitDec *output;

    assert(op1 != NULL && op2 != NULL);

    //�����󳤶�  ���
    maxLen = (op1->len > op2->len) ? op1->len : op2->len;
    gap = abs(op1->len - op2->len);

    //������ָ��
    L = maxLen == op1->len ? op1 : op2;
    S = maxLen == op1->len ? op2 : op1;

    //��� �ṹ
    rst = (BigNum *) malloc(sizeof(BigNum));


    //����������� ��һλ��Ϊ��λԤ��
    rstLen = maxLen + 1;
    output = (bitDec *) malloc(sizeof(bitDec) * rstLen);

    //��λ ��Ϊ��λԤ��
    output[0] = 0;

    //�ǽ�λ���
    long long i;
    for (i = 0; i < gap; i++) {
        output[i + 1] = L->numer[i];
    }
    for (i = gap; i < maxLen; i++) {
        //rst[i]=l[i]+s[i-gap]
        output[i + 1] = L->numer[i] + S->numer[i - gap];
    }

    //ѭ����λ����
    for (i = maxLen; i > 0; i--) {
        if (output[i] >= 10) {
            output[i] -= 10;
            output[i - 1] += 1;
        }
    }

    //��λ0�ж�
    if (output[0] == 0) {
        output++;
        rstLen--;
    }

    //�����ֵ
    memcpy(rst->numer, output, rstLen);
    memset(&(rst->numer[rstLen]), NOT_NUM, MAX_BIT_LEN - rstLen);
    rst->sign = POS;//����λΪ��
    rst->len = rstLen;

    //�ͷžֲ���Դ
//    free(output);

    return rst;
}

/**
 * �ײ��޷��ż���
 * @param op1 ������1
 * @param op2 ����2
 * @return ������
 */
BigNum *BigNum_Unsigned_SUB(BigNum *op1, BigNum *op2) {
    unsigned long long maxLen = 0, gap, Zero_cnt = 0;
    BigNum *rst, *L, *S;
    bitDec *output;

    assert(op1 != NULL && op2 != NULL);

    //�����󳤶�  ���
    maxLen = (op1->len > op2->len) ? op1->len : op2->len;
    gap = abs(op1->len - op2->len);

    //������ָ��
    L = maxLen == op1->len ? op1 : op2;
    S = maxLen == op1->len ? op2 : op1;

    //��� �ṹ
    rst = (BigNum *) malloc(sizeof(BigNum));

    //�����������
    output = (bitDec *) malloc(sizeof(bitDec) * (maxLen));

    long long i, j;
    for (i = 0; i < gap; i++) {
        output[i] = L->numer[i];
    }
    for (i = gap; i < maxLen; i++) {
        //rst[i]=l[i]-s[i-gap]
        output[i] = L->numer[i] - S->numer[i - gap];
    }

    //ѭ����λ����
    for (i = maxLen - 1; i > 0; i--) {
        if (output[i] < 0) {
            output[i] += 10;
            output[i - 1] -= 1;
        }
    }


    //��λ0ͳ��
    for (i = 0; output[i] == 0 && i < maxLen; i++) {
        Zero_cnt++;
    }
    //ȫΪ0
    if (Zero_cnt == maxLen) {
        //���0
        rst->numer[0] = 0;
        memset(&(rst->numer[1]), NOT_NUM, MAX_BIT_LEN - 1);
        rst->len = 1;
    } else {
        for (i = Zero_cnt, j = 0; i < maxLen; i++, j++) {
            rst->numer[j] = output[i];
        }
        //memcpy(rst->numer,&output[Zero_cnt],maxLen-Zero_cnt);
        memset(&(rst->numer[j]), NOT_NUM, MAX_BIT_LEN - maxLen + Zero_cnt);
        rst->len = maxLen - Zero_cnt;
    }

    //����λ��ֵ
    rst->sign = L == op1 ? POS : NEG;

    //�ͷžֲ���Դ
//	free(output);


    return rst;
}

/**
 * �����ӷ���С���㴦�����õײ����㣩
 * @param op1 ����1
 * @param op2 ����2
 * @return ������
 */
BigNum *BigNum_ADD(BigNum *op1, BigNum *op2) {
    BigNum *rst;

    assert(op1 != NULL && op2 != NULL);

    // С�������
    VeryLongInt offset = 0;

    if (op1->point > op2->point) {
        unsigned long long gap = offset = op1->point - op2->point;
        op2->offset = 1;
        for (int i = 0; i < gap; ++i) {
            op2->numer[op2->len] = 0;
            op2->len++;
            op2->point++;
        }
    } else if (op1->point < op2->point) {
        unsigned long long gap = offset = op2->point - op1->point;
        op1->offset = 1;
        for (int i = 0; i < gap; ++i) {
            op1->numer[op1->len] = 0;
            op1->len++;
            op1->point++;
        }
    }


    if (op1->sign == POS && op2->sign == POS) {
        rst = BigNum_Unsigned_ADD(op1, op2);
    } else if (op1->sign == POS && op2->sign == NEG) {
        rst = BigNum_Unsigned_SUB(op1, op2);
    } else if (op1->sign == NEG && op2->sign == POS) {
        rst = BigNum_Unsigned_SUB(op2, op1);
    } else if (op1->sign == NEG && op2->sign == NEG) {
        rst = BigNum_Unsigned_ADD(op1, op2);
        rst->sign = NEG;
    }


    rst->point = op1->point;

    // ��ԭС�������
    if (op1->offset == 1) {
        memset(&(op1->numer[op1->len - offset]), NOT_NUM, offset);
        op1->len -= offset;
        op1->point -= offset;
        op1->offset = 0;
    }
    if (op2->offset == 1) {
        memset(&(op2->numer[op2->len - offset]), NOT_NUM, offset);
        op2->len -= offset;
        op2->point -= offset;
        op2->offset = 0;
    }
    rst->offset = 0;
    return rst;
}

/**
 * ����������С���㴦�����õײ����㣩
 * @param op1 ������
 * @param op2 ����
 * @return ������
 */
BigNum *BigNum_SUB(BigNum *op1, BigNum *op2) {
    BigNum *rst;

    assert(op1 != NULL && op2 != NULL);

    VeryLongInt offset = 0;
    if (op1->point > op2->point) {
        unsigned long long gap = offset = op1->point - op2->point;
        op2->offset = 1;
        for (int i = 0; i < gap; ++i) {
            op2->numer[op2->len] = 0;
            op2->len++;
            op2->point++;
        }
    } else if (op1->point < op2->point) {
        unsigned long long gap = offset = op2->point - op1->point;
        op1->offset = 1;
        for (int i = 0; i < gap; ++i) {
            op1->numer[op1->len] = 0;
            op1->len++;
            op1->point++;
        }
    }

    if (op1->sign == POS && op2->sign == POS) {
        int tmp = 0;
        for (unsigned long long i = 0; i < op1->len; ++i) {
            if (op1->numer[i] > op2->numer[i]) {
                rst = BigNum_Unsigned_SUB(op1, op2);
                rst->sign = 1;
                tmp = 1;
                break;
            }
        }
        if (!tmp) {
            rst = BigNum_Unsigned_SUB(op2, op1);
            rst->sign = 0;
        }
    } else if (op1->sign == POS && op2->sign == NEG) {
        rst = BigNum_Unsigned_ADD(op1, op2);
    } else if (op1->sign == NEG && op2->sign == POS) {
        rst = BigNum_Unsigned_ADD(op1, op2);
        rst->sign = NEG;
    } else if (op1->sign == NEG && op2->sign == NEG) {
        rst = BigNum_Unsigned_SUB(op2, op1);
    }


    rst->point = op1->point;

    // ��ԭС�������
    if (op1->offset == 1) {
        memset(&(op1->numer[op1->len - offset]), NOT_NUM, offset);
        op1->len -= offset;
        op1->point -= offset;
        op1->offset = 0;
    }
    if (op2->offset == 1) {
        memset(&(op2->numer[op2->len - offset]), NOT_NUM, offset);
        op2->len -= offset;
        op2->point -= offset;
        op2->offset = 0;
    }
    rst->offset = 0;
    return rst;
}

/**
 * �����˷�
 * @param op1 ����1
 * @param op2 ����2
 * @return ������
 */
BigNum *BigNum_MUL(BigNum *op1, BigNum *op2) {
    unsigned long long maxLen, minLen, rstLen;
    BigNum *rst, *L, *S;
    bitDec *output;
    long long i, j;

    assert(op1 != NULL && op2 != NULL);

    //���� ��̳���
    maxLen = (op1->len > op2->len) ? op1->len : op2->len;
    minLen = (op1->len == maxLen) ? op2->len : op1->len;

    //������� Ԥ��һλ��λ
    rstLen = maxLen + minLen;


    //������ָ��
    L = maxLen == op1->len ? op1 : op2;
    S = maxLen == op1->len ? op2 : op1;

    //��� �ṹ
    rst = (BigNum *) malloc(sizeof(BigNum));

    //�����������
    output = (bitDec *) malloc(sizeof(bitDec) * (rstLen));
    memset(output, 0, rstLen);


    for (i = minLen - 1; i >= 0; i--) {
        //�ǽ���
        for (j = maxLen - 1; j >= 0; j--) {
            output[i + j + 1] += L->numer[j] * S->numer[i];
        }


        // ��λ
        for (j = maxLen + i; j > i; j--) {
            output[j - 1] += output[j] / 10;
            output[j] = output[j] % 10;
        }

    }

    //Ԥ���׽�λ�ж�
    if (output[0] == 0) {
        rstLen--;
        output++;
    }

    //��ֵ��rst
    memcpy(rst->numer, output, rstLen);
    memset(&(rst->numer[rstLen]), NOT_NUM, MAX_BIT_LEN - rstLen);
    rst->sign = (op1->sign == op2->sign) ? POS : NEG;
    rst->len = rstLen;

    //�ͷžֲ���Դ
//	free(output);

    rst->point = op1->point + op2->point;

    return rst;
}

/**
 * ��������
 * @param op1 ������
 * @param op2 ����
 * @param tail ���ٵı���λ��
 * @return ������
 */
BigNum *BigNum_DIV(BigNum *op1, BigNum *op2, VeryLongInt tail) {
    unsigned long long rstLen, len1, len2;
    BigNum *rst;
    bitDec *output, *subc;
    long long i, j, k;
    char s0_flag = 0;

    assert(op1 != NULL && op2 != NULL);

    unsigned long long gap = op1->point + op2->point;
    unsigned long long max = (gap + tail <= MAX_BIT_LEN * 2 - 1) ? tail : MAX_BIT_LEN * 2 - 1 - gap;
    for (unsigned long long kk = 0; kk < max; kk++) {
        op1->numer[op1->len] = 0;
        op1->len++;
        op1->point++;
    }


    //������� ͬʱԤ��һ����λ 
    rstLen = op1->len - op2->len + 1;
    len1 = op1->len;
    len2 = op2->len;

    //��� �ṹ 
    rst = (BigNum *) malloc(sizeof(BigNum));

    //�����������
    output = (bitDec *) malloc(sizeof(bitDec) * (rstLen));
    memset(output, 0, rstLen);

    //�������
    subc = (bitDec *) malloc(sizeof(bitDec) * (len2 + 1));
    memset(subc, 0, len2 + 1);


    for (i = 0; i < rstLen; i++) {
        if (i == 0) {
            //��һ��ȡǰ��op1ȡlen2���� subc��λΪ0
            for (j = 0; j < len2; j++) {
                subc[j + 1] = op1->numer[j];
            }
        } else {
            //�ڶ��ο�ʼ
            //ǰlen2����λ���ӻ�
            for (j = 0; j < len2; j++) {
                subc[j] = subc[j + 1] + op2->numer[j];
            }
            //����һ������
            subc[len2] = op1->numer[i + len2 - 1];
        }
        //1-10�Լ�
        for (j = 1; j <= 10; j++) {
            //����
            for (k = 0; k < len2; k++) {
                subc[k + 1] = subc[k + 1] - op2->numer[k];
            }

            //С��0�ж�
            s0_flag = 0; //�Ƿ�С��0
            for (k = 0; k < len2 + 1; k++) {
                //��һ��������Ϊ���� ��һ��Ϊ����
                if (subc[k] < 0) {
                    s0_flag = 1;
                    break;
                }
                    //��һ��������Ϊ���� ��һ��Ϊ����
                else if (subc[k] > 0) {
                    break;
                }
            }
            //С��0 дoutput
            if (s0_flag) {
                output[i] = j - 1;
                break;
            }
            //���� ��������
            for (k = len2; k > 0; k--) {
                if (subc[k] < 0) {
                    subc[k] += 10;
                    subc[k - 1]--;
                }
            }
        }
    }

    //�����λ�ж�
    if (output[0] == 0) {
        output++;
        rstLen--;
    }

    //д��
    memcpy(rst->numer, output, rstLen);
    memset(&(rst->numer[rstLen]), NOT_NUM, MAX_BIT_LEN * 2 - 1 - rstLen);
    rst->sign = (op1->sign == op2->sign) ? POS : NEG;
    rst->len = rstLen;

    //�ͷžֲ���Դ
//	free(output);
//	free(subc);

    rst->point = op1->point - op2->point;
    rst->offset = 0;
    for (unsigned long long kk = 0; kk < max; kk++) {
        op1->numer[op1->len - 1] = NOT_NUM;
        op1->len--;
        op1->point--;
    }

    return rst;
}

/**
 * ����С���������0
 * @param BigNum ��Ҫ��ϴ������
 * @return ԭ���ݽṹ
 */
BigNum *Clear(BigNum *BigNum) {
    while (BigNum->point > 0 && BigNum->numer[BigNum->len - 1] == 0) {
        BigNum->numer[BigNum->len - 1] = NOT_NUM;
        BigNum->point--;
        BigNum->len--;
    }
    if (BigNum->len == 1 && BigNum->numer[0] == 0)
        BigNum->sign = POS;
    return BigNum;
}

/**
 * �����Ƚ�
 * @param op1 �Ƚ���1
 * @param op2 �Ƚ���2
 * @return 1 op1>op2 0 op1=op2 -1 op1<op2
 */
int Compare(BigNum *op1, BigNum *op2) {
    BigNum *rst = BigNum_SUB(op1, op2);
    rst = Clear(rst);
    if (rst->sign == NEG)return -1;
    else {
        if (rst->len == 1 && rst->numer[0] == 0)return 0;
        else return 1;
    }
}
