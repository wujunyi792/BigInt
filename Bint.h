# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# include<math.h>
# include <assert.h>

/******************************************************************
 关于 使用char表示一个十进制数有关的一些宏定义 
*******************************************************************/

#define ASCII_NUM_9 57
#define ASCII_NUM_0 48
#define ASCII_NUM_POINTER 46
#define POS  1
#define NEG  0
#define  Fix(n) ((n)-ASCII_NUM_0)
#define NOT_NUM 0x88

/******************************************************************
 一个大整数最长的位数 
*******************************************************************/
#ifndef MAX_BIT_LEN
#define MAX_BIT_LEN 500
#endif

/******************************************************************
  数据结构定义 
*******************************************************************/
typedef char bitDec;

typedef struct intSave {
    bitDec numer[MAX_BIT_LEN * 2 + 1];//十进制数字
    char sign;//符号位
    unsigned long long point; //小数点位置
    unsigned long long len; //长度
    int offset; //是否有扩零
} BINT_t;

/******************************************************************
 函数声明 
*******************************************************************/
//交互  
BINT_t *Bint_input(char hint[]);

void Bint_output(BINT_t *Bint);

//简化使用定义 
#define BintGet(c)  (Bint_input((c)))
#define BintShow(b)  (Bint_output((b)))

//无符号加减 
BINT_t *Bint_Unsigned_ADD(BINT_t *op1, BINT_t *op2);

BINT_t *Bint_Unsigned_SUB(BINT_t *op1, BINT_t *op2);

//有符号加减
BINT_t *Bint_ADD(BINT_t *op1, BINT_t *op2);

BINT_t *Bint_SUB(BINT_t *op1, BINT_t *op2);

//乘法 除法 
BINT_t *Bint_MUL(BINT_t *op1, BINT_t *op2);

BINT_t *Bint_DIV(BINT_t *op1, BINT_t *op2, unsigned long long int tail);


//////////////////////////////////具体函数//////////////////////////////////////
//               包括 无符号加减 有符号加减 乘除 模余                        // 
/////////////////////////////////////////////////////////////////////////////// 

/******************************************************
* Des: 从用户输入获得一个大整数结构 
* @Argu:字符串 [提示信息] 
* Returns:指针  大整数结构 
*******************************************************/
BINT_t *Bint_input(char hint[]) {
    //输入的缓存 
    char input[MAX_BIT_LEN + 1];

    //大数结构数组
    BINT_t *ret = (BINT_t *) malloc(sizeof(BINT_t));
    ret->point = 0;

    //交互
    printf("[%s] 请输入一个大数(%d位以内)\n", hint, MAX_BIT_LEN);
    scanf("%s", input);

    //游标  计算长度
    char *p = input;

    /*符号判断*/
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
    /*计算长度并赋值*/
    int len_cnt = 0;
    char begin_flag = 1;
    int havePointer = 0;
    while (*p != '\0') {
        if (*p > ASCII_NUM_9 || (*p < ASCII_NUM_0 && *p != ASCII_NUM_POINTER)) {
            printf(":::输入错误 非数字\n");
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
    //全为0
    if (*p == '\0' && begin_flag == 1) {
        ret->numer[0] = 0;
        ret->len = len_cnt = 1;
    }

    //置len位
    ret->len = len_cnt;
    ret->offset = 0;

    //设置剩余的为0x88 -120
    memset(&(ret->numer[len_cnt]), NOT_NUM, MAX_BIT_LEN - len_cnt);

    return ret;
}


/******************************************************
* Des: 把BINT转化为数字形式输出 
* @Argu:BINT_t* 要输出的数字 
* @Returns:void 
*******************************************************/
void Bint_output(BINT_t *Bint) {

    if (Bint->sign == NEG) {
        printf("-");
    }
    long long i;
    int havePointer = Bint->point != -1;
    if (Bint->len <= Bint->point) {
        printf("0.");
        for (int j = 0; j < Bint->point - Bint->len; ++j) {
            printf("0");
        }
        havePointer = 0;
    }

    for (i = 0; i < Bint->len; i++) {
        if (i == Bint->len - Bint->point && havePointer) {
            printf(".");
            havePointer = 0;
            i--;
            continue;
        }
        printf("%d", Bint->numer[i]);
    }

    printf("\n");
}

/******************************************************
* Des: 无符号的加法 
* @Argu:BINT_t* 加数
*       BINT_t* 加数 
* @Returns: BINT* 相加后的结果 sign 默认为正  
*******************************************************/
BINT_t *Bint_Unsigned_ADD(BINT_t *op1, BINT_t *op2) {
    unsigned long long maxLen, rstLen, gap;
    BINT_t *rst, *L, *S;
    bitDec *output;

    assert(op1 != NULL && op2 != NULL);

    //获得最大长度  间距
    maxLen = (op1->len > op2->len) ? op1->len : op2->len;
    gap = abs(op1->len - op2->len);

    //最长与最短指针
    L = maxLen == op1->len ? op1 : op2;
    S = maxLen == op1->len ? op2 : op1;

    //结果 结构
    rst = (BINT_t *) malloc(sizeof(BINT_t));


    //结果缓存数组 留一位作为进位预留
    rstLen = maxLen + 1;
    output = (bitDec *) malloc(sizeof(bitDec) * rstLen);

    //首位 作为进位预留
    output[0] = 0;

    //非进位相加
    long long i;
    for (i = 0; i < gap; i++) {
        output[i + 1] = L->numer[i];
    }
    for (i = gap; i < maxLen; i++) {
        //rst[i]=l[i]+s[i-gap]
        output[i + 1] = L->numer[i] + S->numer[i - gap];
    }

    //循环进位处理
    for (i = maxLen; i > 0; i--) {
        if (output[i] >= 10) {
            output[i] -= 10;
            output[i - 1] += 1;
        }
    }

    //首位0判断
    if (output[0] == 0) {
        output++;
        rstLen--;
    }

    //输出赋值
    memcpy(rst->numer, output, rstLen);
    memset(&(rst->numer[rstLen]), NOT_NUM, MAX_BIT_LEN - rstLen);
    rst->sign = POS;//符号位为正
    rst->len = rstLen;

    //释放局部资源
//	free(output);

    return rst;
}

/******************************************************
* Des: 无符号 减法 
* @Argu:BINT_t* op1被减数 op2减数 
* @Returns:BINT_t 相加后的结果  符号位依据结果而定 
*******************************************************/
BINT_t *Bint_Unsigned_SUB(BINT_t *op1, BINT_t *op2) {
    unsigned long long maxLen = 0, gap, Zero_cnt = 0;
    BINT_t *rst, *L, *S;
    bitDec *output;

    assert(op1 != NULL && op2 != NULL);

    //获得最大长度  间距
    maxLen = (op1->len > op2->len) ? op1->len : op2->len;
    gap = abs(op1->len - op2->len);

    //最长与最短指针
    L = maxLen == op1->len ? op1 : op2;
    S = maxLen == op1->len ? op2 : op1;

    //结果 结构
    rst = (BINT_t *) malloc(sizeof(BINT_t));

    //结果缓存数组
    output = (bitDec *) malloc(sizeof(bitDec) * (maxLen));

    long long i, j;
    for (i = 0; i < gap; i++) {
        output[i] = L->numer[i];
    }
    for (i = gap; i < maxLen; i++) {
        //rst[i]=l[i]-s[i-gap]
        output[i] = L->numer[i] - S->numer[i - gap];
    }

    //循环进位处理
    for (i = maxLen - 1; i > 0; i--) {
        if (output[i] < 0) {
            output[i] += 10;
            output[i - 1] -= 1;
        }
    }


    //首位0统计
    for (i = 0; output[i] == 0 && i < maxLen; i++) {
        Zero_cnt++;
    }
    //全为0
    if (Zero_cnt == maxLen) {
        //输出0
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

    //符号位赋值
    rst->sign = L == op1 ? POS : NEG;

    //释放局部资源
//	free(output);


    return rst;
}


/******************************************************
* Des: 带符号 大整数加法 
* @Argu: BINT_t* 
*        BINT_t* 要相加的两个大整数 
* @Returns: BINT_t* 加后的结果 
*******************************************************/
BINT_t *Bint_ADD(BINT_t *op1, BINT_t *op2) {
    BINT_t *rst;

    assert(op1 != NULL && op2 != NULL);

    // 小数点对齐
    unsigned long long int offset = 0;

    if (op1->point > op2->point) {
        unsigned long long gap = offset = op1->point - op2->point;
        op2->offset = 1;
        for (int i = 0; i < gap; ++i) {
            op2->numer[op2->len] = 0;
            op2->len++;
            op2->point++;
        }
    } else if (op1->len < op2->len) {
        unsigned long long gap = offset = op2->point - op1->point;
        op1->offset = 1;
        for (int i = 0; i < gap; ++i) {
            op1->numer[op1->len] = 0;
            op1->len++;
            op1->point++;
        }
    }


    if (op1->sign == POS && op2->sign == POS) {
        rst = Bint_Unsigned_ADD(op1, op2);
    } else if (op1->sign == POS && op2->sign == NEG) {
        rst = Bint_Unsigned_SUB(op1, op2);
    } else if (op1->sign == NEG && op2->sign == POS) {
        rst = Bint_Unsigned_SUB(op2, op1);
    } else if (op1->sign == NEG && op2->sign == NEG) {
        rst = Bint_Unsigned_ADD(op1, op2);
        rst->sign = NEG;
    }


    rst->point = op1->point;

    // 还原小数点对齐
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

    return rst;
}

/******************************************************
* Des: 大整数减法  op1-op2 
* @Argu: BINT_t* 被减数
*        BINT_t* 减数 
* @Returns:BINT_t* 结果 
*******************************************************/
BINT_t *Bint_SUB(BINT_t *op1, BINT_t *op2) {
    BINT_t *rst;

    assert(op1 != NULL && op2 != NULL);

    unsigned long long int offset = 0;
    if (op1->point > op2->point) {
        unsigned long long gap = offset = op1->point - op2->point;
        op2->offset = 1;
        for (int i = 0; i < gap; ++i) {
            op2->numer[op2->len] = 0;
            op2->len++;
            op2->point++;
        }
    } else if (op1->len < op2->len) {
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
                rst = Bint_Unsigned_SUB(op1, op2);
                rst->sign = 1;
                tmp = 1;
                break;
            }
        }
        if (!tmp) {
            rst = Bint_Unsigned_SUB(op2, op1);
            rst->sign = 0;
        }
    } else if (op1->sign == POS && op2->sign == NEG) {
        rst = Bint_Unsigned_ADD(op1, op2);
    } else if (op1->sign == NEG && op2->sign == POS) {
        rst = Bint_Unsigned_ADD(op1, op2);
        rst->sign = NEG;
    } else if (op1->sign == NEG && op2->sign == NEG) {
        rst = Bint_Unsigned_SUB(op2, op1);
    }


    rst->point = op1->point;

    // 还原小数点对齐
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
    return rst;
}

/******************************************************
* Des: 大整数乘法 
* @Argu: BINT_t* op1 op2 两个乘数 
* @Returns:BINT_t* 乘法结果 
*******************************************************/
BINT_t *Bint_MUL(BINT_t *op1, BINT_t *op2) {
    unsigned long long maxLen, minLen, rstLen;
    BINT_t *rst, *L, *S;
    bitDec *output;
    long long i, j;

    assert(op1 != NULL && op2 != NULL);

    //获得最长 最短长度
    maxLen = (op1->len > op2->len) ? op1->len : op2->len;
    minLen = (op1->len == maxLen) ? op2->len : op1->len;

    //结果长度 预留一位进位
    rstLen = maxLen + minLen;


    //最长与最短指针
    L = maxLen == op1->len ? op1 : op2;
    S = maxLen == op1->len ? op2 : op1;

    //结果 结构
    rst = (BINT_t *) malloc(sizeof(BINT_t));

    //结果缓存数组
    output = (bitDec *) malloc(sizeof(bitDec) * (rstLen));
    memset(output, 0, rstLen);


    for (i = minLen - 1; i >= 0; i--) {
        //非进制
        for (j = maxLen - 1; j >= 0; j--) {
            output[i + j + 1] += L->numer[j] * S->numer[i];
        }


        // 进位
        for (j = maxLen + i; j > i; j--) {
            output[j - 1] += output[j] / 10;
            output[j] = output[j] % 10;
        }

    }

    //预留首进位判断
    if (output[0] == 0) {
        rstLen--;
        output++;
    }

    //赋值给rst
    memcpy(rst->numer, output, rstLen);
    memset(&(rst->numer[rstLen]), NOT_NUM, MAX_BIT_LEN - rstLen);
    rst->sign = (op1->sign == op2->sign) ? POS : NEG;
    rst->len = rstLen;

    //释放局部资源
//	free(output);


    rst->point = op1->point + op2->point;
    return rst;
}

/******************************************************
* Des: 大整数除法 
* @Argu: BINT_t* op1[被除数]   op2[除数]   
* @Returns:BINT_t*  结果  取整 
*******************************************************/
BINT_t *Bint_DIV(BINT_t *op1, BINT_t *op2, unsigned long long int tail) {
    unsigned long long rstLen, len1, len2;
    BINT_t *rst;
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


    //结果长度 同时预留一个进位 
    rstLen = op1->len - op2->len + 1;
    len1 = op1->len;
    len2 = op2->len;

    //结果 结构 
    rst = (BINT_t *) malloc(sizeof(BINT_t));

    //结果缓存数组
    output = (bitDec *) malloc(sizeof(bitDec) * (rstLen));
    memset(output, 0, rstLen);

    //相减缓存
    subc = (bitDec *) malloc(sizeof(bitDec) * (len2 + 1));
    memset(subc, 0, len2 + 1);


    for (i = 0; i < rstLen; i++) {
        if (i == 0) {
            //第一次取前从op1取len2个数 subc首位为0
            for (j = 0; j < len2; j++) {
                subc[j + 1] = op1->numer[j];
            }
        } else {
            //第二次开始
            //前len2个移位并加回
            for (j = 0; j < len2; j++) {
                subc[j] = subc[j + 1] + op2->numer[j];
            }
            //补充一个进来
            subc[len2] = op1->numer[i + len2 - 1];
        }
        //1-10试减
        for (j = 1; j <= 10; j++) {
            //减法
            for (k = 0; k < len2; k++) {
                subc[k + 1] = subc[k + 1] - op2->numer[k];
            }

            //小于0判断
            s0_flag = 0; //是否小于0
            for (k = 0; k < len2 + 1; k++) {
                //第一个非零数为负数 则一定为负数
                if (subc[k] < 0) {
                    s0_flag = 1;
                    break;
                }
                    //第一个非零数为正数 则一定为正数
                else if (subc[k] > 0) {
                    break;
                }
            }
            //小于0 写output
            if (s0_flag) {
                output[i] = j - 1;
                break;
            }
            //否则 负数进制
            for (k = len2; k > 0; k--) {
                if (subc[k] < 0) {
                    subc[k] += 10;
                    subc[k - 1]--;
                }
            }
        }
    }

    //结果首位判断
    if (output[0] == 0) {
        output++;
        rstLen--;
    }

    //写出
    memcpy(rst->numer, output, rstLen);
    memset(&(rst->numer[rstLen]), NOT_NUM, MAX_BIT_LEN * 2 - 1 - rstLen);
    rst->sign = (op1->sign == op2->sign) ? POS : NEG;
    rst->len = rstLen;

    //释放局部资源
//	free(output);
//	free(subc);

    rst->point = op1->point - op2->point;

    return rst;
}
