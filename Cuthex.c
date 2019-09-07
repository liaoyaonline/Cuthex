#include <stdio.h>//梳理结构，优化代码
#include <errno.h>
#include <string.h>
#include <stdlib.h>
typedef struct Node
{
    char inputfilename[100];
    char outputfilename[20];
    char headaddress[20];
    char endaddress[20];
    struct Node *next;
}ListNode;
int N;
void init(ListNode* head,char inputfilename[]);
void inittest(ListNode* head);
void cut(ListNode* head);
int main(int argc,char** argv)//获取hex文件名,argv[1]
{
    ListNode *phead = (ListNode*)malloc(sizeof(ListNode));
    init(phead,argv[1]);
    inittest(phead);
    printf("%s        %s\n",phead->headaddress,phead->endaddress);
    int i = 0;
    while(i < N)
    {
        cut(phead);
        phead = phead->next;
        i++;
    }
    return 0;
}
void init(ListNode* head,char inputfilename[])//读取配置文件，将其转化成方便计算的值，存储在单向链表
{
    ListNode *p = head;
    FILE *fp = NULL;
    fp = fopen("config.txt","r");
    fscanf(fp,"%d",&N);
    printf("%d\n",N);
    fscanf(fp,"%s%s",p->headaddress,p->endaddress);
    p->endaddress[6] = p->endaddress[6] -1;//结尾地址需要更改，因为每次写都是写一行，也就是20byte
    p->endaddress[7] = '0';
    char outputfilename[20] = "VBF1.hex";
    memset(p->inputfilename,0,sizeof(p->inputfilename));//所有位置0，避免出现读写出现错误
    strcpy(p->inputfilename,inputfilename);
    strcpy(p->outputfilename,outputfilename);
    printf("%s        %s\n",p->headaddress,p->endaddress);
    for(int i = 1;i < N;i++)
    {
        ListNode *q = (ListNode*)malloc(sizeof(ListNode));
        p->next = q;
        p = p->next;
        memset(p->inputfilename,0,sizeof(p->inputfilename));
        memset(p->outputfilename,0,sizeof(p->outputfilename));
        strcpy(p->inputfilename,inputfilename);
        strcpy(p->outputfilename,outputfilename);
        p->outputfilename[3] = p->outputfilename[3] + i;
        fscanf(fp,"%s%s",p->headaddress,p->endaddress);
        p->endaddress[6] = p->endaddress[6] -1;
        p->endaddress[7] = '0';
        printf("%s      %s\n",p->headaddress,p->endaddress);
    }
    p->next = NULL;
    fclose(fp);
}
void inittest(ListNode* head)//测试是否读取配置正确
{
    ListNode* q = head;
    int i = 0;
    if(head == NULL)
    {
        printf("00000000000000000000000000");
    }
    while(i < N)
    {
        printf("第%d个节点:输入文件：%s,输出文件：%s,开始地址:%s结束地址%s\n",i,q->inputfilename,q->outputfilename,q->headaddress,q->endaddress);
        q = q->next;
        i++;
    }
}
void cut(ListNode* head)
{
    ListNode *phead = head;
    FILE *p = fopen(phead->inputfilename,"r");
    if (p == NULL)
    {
        printf("Message: %s\n",strerror(errno));
    }
    char headaddress[9];
    char endaddress[9];
    char tmp[9];//存储所扫描到的地址
    char tmpsource[20];//15+1;存储目标地址
    char buf[100];//存储扫描到那一行的全部数据
    int i = 0;
    strcpy(headaddress,phead->headaddress);
    strcpy(endaddress,phead->endaddress);
    memset(tmp,0,sizeof(tmp));
    memset(tmpsource,0,sizeof(tmpsource));
    memset(buf,0,sizeof(buf));
    printf("\n第一次测试：开始地址：%s当前地址：%s\n",headaddress,tmp);
    while(strcmp(headaddress,tmp) != 0)
    {
        memset(buf,0,sizeof(buf));
        fscanf(p,"%s",buf);
        printf("第%d次循环：%s\n",i,buf);
        if(buf[8] == '4')//更新地址
        {
            strcpy(tmpsource,buf);
            tmp[0] = buf[9];
            tmp[1] = buf[10];
            tmp[2] = buf[11];
            tmp[3] = buf[12];
            fscanf(p,"%s",buf);
            tmp[4] = buf[3];
            tmp[5] = buf[4];
            tmp[6] = buf[5];
            tmp[7] = buf[6];
        }
        else if(buf[8] == '0')
        {
            tmp[4] = buf[3];
            tmp[5] = buf[4];
            tmp[6] = buf[5];
            tmp[7] = buf[6];
        }
        i++;
    }
    FILE *fw = fopen(phead->outputfilename,"w");//名字可以用字符串替代，比如tmp
    i = 0;
    printf("第%d次循环：%s\n",i,buf);
    fprintf(fw,"%s\n",tmpsource);
    while(strcmp(tmp,endaddress) !=0)
    {
        fprintf(fw,"%s\n",buf);
        fscanf(p,"%s",buf);
        printf("第%d次循环：%s\n",i,buf);
        if(buf[8] == '4')//更新地址
        {
            strcpy(tmpsource,buf);
            fprintf(fw,"%s\n",tmpsource);
            tmp[0] = buf[9];
            tmp[1] = buf[10];
            tmp[2] = buf[11];
            tmp[3] = buf[12];
            fscanf(p,"%s",buf);
            tmp[4] = buf[3];
            tmp[5] = buf[4];
            tmp[6] = buf[5];
            tmp[7] = buf[6];
        }
        else if(buf[8] == '0')
        {
            tmp[4] = buf[3];
            tmp[5] = buf[4];
            tmp[6] = buf[5];
            tmp[7] = buf[6];
        }
        i++;
    }
    char endflag[12] = ":00000001FF";
    fprintf(fw,"%s\n",buf);
    fprintf(fw,"%s",endflag);
    fclose(fw);
}
