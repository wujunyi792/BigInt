# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <assert.h>

/**
 * num -> char 相关定义
 */
#define ASCII_NUM_9 57
#define ASCII_NUM_0 48
#define ASCII_NUM_POINTER 46  //小数点
#define POS  1
#define NEG  0
#define  Fix(n) ((n)-ASCII_NUM_0)
#define NOT_NUM 0x88

/**
 * 定义最长字符串长度支持
 */
#ifndef MAX_BIT_LEN
#define MAX_BIT_LEN 500
#endif

/**
 * 大数结构体定义
 */
typedef char bitDec;
typedef unsigned long long int VeryLongInt; //内部计算时会用到，避免每次都写一边 unsigned long long int
typedef struct numberStr {
    bitDec numer[MAX_BIT_LEN * 2 + 1];//十进制数字, *2-1防止相乘溢出
    char sign;//符号位
    unsigned long long point; //小数点位置
    unsigned long long len; //长度
    int offset; //是否有扩零
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
 * Des: 从用户输入生成一个大数结构
 * @Argu:字符串 [提示信息] 
 * Returns:指针  大数结构
 */
BigNum *MakeNumFromIO() {
    //输入的缓存 
    char input[MAX_BIT_LEN + 1];

    //大数结构数组
    BigNum *ret = (BigNum *) malloc(sizeof(BigNum));
    ret->point = 0;

    //交互
    printf("[%s] 请输入一个大数(%d位以内)\n", "操作数", MAX_BIT_LEN);
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

/**
 * Des: 从字符串解析一个大数结构
 * @Argu 字符串 [要解析的字符串]
 * Returns:指针  大数结构
 */
BigNum *MakeNum(char str[]) {
    //输入的缓存
    char input[MAX_BIT_LEN + 1];

    //大数结构数组
    BigNum *ret = (BigNum *) malloc(sizeof(BigNum));
    ret->point = 0;

    //游标  计算长度
    char *p = str;

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

/**
 * 数字输出
 * @param BigNum 要输出的数字
 */
void ShowNum(BigNum *BigNum) {

    // 去除小数点
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
 * 底层无符号加法
 * @param op1 加数1
 * @param op2 加数2
 * @return 运算结果
 */
BigNum *BigNum_Unsigned_ADD(BigNum *op1, BigNum *op2) {
    unsigned long long maxLen, rstLen, gap;
    BigNum *rst, *L, *S;
    bitDec *output;

    assert(op1 != NULL && op2 != NULL);

    //获得最大长度  间距
    maxLen = (op1->len > op2->len) ? op1->len : op2->len;
    gap = abs(op1->len - op2->len);

    //最长与最短指针
    L = maxLen == op1->len ? op1 : op2;
    S = maxLen == op1->len ? op2 : op1;

    //结果 结构
    rst = (BigNum *) malloc(sizeof(BigNum));


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
//    free(output);

    return rst;
}

/**
 * 底层无符号减法
 * @param op1 被减数1
 * @param op2 减数2
 * @return 运算结果
 */
BigNum *BigNum_Unsigned_SUB(BigNum *op1, BigNum *op2) {
    unsigned long long maxLen = 0, gap, Zero_cnt = 0;
    BigNum *rst, *L, *S;
    bitDec *output;

    assert(op1 != NULL && op2 != NULL);

    //获得最大长度  间距
    maxLen = (op1->len > op2->len) ? op1->len : op2->len;
    gap = abs(op1->len - op2->len);

    //最长与最短指针
    L = maxLen == op1->len ? op1 : op2;
    S = maxLen == op1->len ? op2 : op1;

    //结果 结构
    rst = (BigNum *) malloc(sizeof(BigNum));

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

/**
 * 整数加法（小数点处理，调用底层运算）
 * @param op1 加数1
 * @param op2 加数2
 * @return 运算结果
 */
BigNum *BigNum_ADD(BigNum *op1, BigNum *op2) {
    BigNum *rst;

    assert(op1 != NULL && op2 != NULL);

    // 小数点对齐
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
    rst->offset = 0;
    return rst;
}

/**
 * 大数减法（小数点处理，调用底层运算）
 * @param op1 被减数
 * @param op2 减数
 * @return 运算结果
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
    rst->offset = 0;
    return rst;
}

/**
 * 大数乘法
 * @param op1 乘数1
 * @param op2 乘数2
 * @return 运算结果
 */
BigNum *BigNum_MUL(BigNum *op1, BigNum *op2) {
    unsigned long long maxLen, minLen, rstLen;
    BigNum *rst, *L, *S;
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
    rst = (BigNum *) malloc(sizeof(BigNum));

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

/**
 * 大数除法
 * @param op1 被除数
 * @param op2 除数
 * @param tail 最少的保留位数
 * @return 运算结果
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


    //结果长度 同时预留一个进位 
    rstLen = op1->len - op2->len + 1;
    len1 = op1->len;
    len2 = op2->len;

    //结果 结构 
    rst = (BigNum *) malloc(sizeof(BigNum));

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
    rst->offset = 0;
    for (unsigned long long kk = 0; kk < max; kk++) {
        op1->numer[op1->len - 1] = NOT_NUM;
        op1->len--;
        op1->point--;
    }

    return rst;
}

/**
 * 清理小数点后无用0
 * @param BigNum 需要清洗的数据
 * @return 原数据结构
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
 * 两数比较
 * @param op1 比较数1
 * @param op2 比较数2
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
