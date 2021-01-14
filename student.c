#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<windows.h>
#include<locale.h>
#include<conio.h>

void mainMenu();
void menu1();
void menu2();
void menu3();
void menu4();
void menu2_2();
void menu2_4();
void menu4_1();
void menu4_2();
void welcomeMenu();
void space(int a);//打空格
void enter(int a);//打回车
void font(wchar_t *p);//
void restoreColor();
void fontColor(int r,int g,int b);
void backgroundColor(int r,int g,int b);
int sortNumber(int limit);
int checkNumber(long long p);
void studentLoginMenu();
void QRcode();

int teacherYes=0;

struct student{
    char name[30];
    long long number;
    int gender;
    float grades;
    float grade[3];
    char password[64];
    short admin;
    struct student *next;//按输入顺序
};

struct tempChain{
    struct student *p;
    struct tempChain *next;
};//用于筛选排序

struct student *locateStudentNumber(long long p,struct student *head);

void editName(struct student *p)//修改姓名
{
    printf ("请输入姓名：");
    begin:
    for (int i=0;i<30;i++)
    {
        p->name[i]=getchar();
        if (p->name[i]=='\n'&&i==0)
        {
            printf ("请重新输入姓名：");
            goto begin;
        }
        if (p->name[i]=='\n')
        {
            p->name[i]='\0';
            goto end;
        }
    }
    fontColor(255,255,255);
    backgroundColor(255,100,0);
    printf ("名字长度超出限制(29字节)");
    restoreColor();
    printf ("\n\n请重新输入姓名：");
    while (getchar()!='\n');
    goto begin;
    end:;
}

void editGender(struct student *p)//修改性别
{
    printf ("\n请输入性别(1为男，2为女)：");
    for (;scanf("%d",&p->gender)!=1;)
    {
        back:
        fontColor(255,255,255);
        backgroundColor(255,100,0);
        printf ("输入格式错误");
        restoreColor();
        printf ("\n\n请重新输入性别：");
        while (getchar()!='\n');
    }
    if (p->gender>2||p->gender<1)
    {
        printf ("输1或者2，谢谢\n");
        goto back;
    }
    if (getchar()!='\n')
    {
        while (getchar()!='\n');
        fontColor(255,255,255);
        backgroundColor(150,100,0);
        printf ("已自动忽略非数字字符后所有内容\n");
        restoreColor();
    }
}

void editNumber(struct student *p)//修改学号
{
    printf ("\n请输入学号：");
    for (;scanf("%lld",&p->number)!=1;)
    {
        back:
        fontColor(255,255,255);
        backgroundColor(255,100,0);
        printf ("输入错误");
        restoreColor();
        printf ("\n\n请重新输入学号：");
        while (getchar()!='\n');
    }
    if (p->number>999999999999||p->number<0)
    {
        printf ("学号为12位正整数\n");
        goto back;
    }
    if (checkNumber(p->number))
    {
        printf ("学号重复\n");
        goto back;
    }
    if (getchar()!='\n')
    {
        while (getchar()!='\n');
        fontColor(255,255,255);
        backgroundColor(150,100,0);
        printf ("已自动忽略非数字字符后所有内容\n");
        restoreColor();
    }
}

void calculateGrades(struct student *p)//计算总分
{
    p->grades=p->grade[0]+p->grade[1]+p->grade[2];
}

void editGrade(struct student *p,int i)//修改成绩
{
    char a[3][5];
    strcpy(a[0],"语文");
    strcpy(a[1],"数学");
    strcpy(a[2],"英语");
    printf ("\n请输入%s成绩：",a[i]);
    for (;scanf("%f",&p->grade[i])!=1;)
    {
        back:
        fontColor(255,255,255);
        backgroundColor(255,100,0);
        printf ("输入错误");
        restoreColor();
        printf ("\n\n请重新输入%s成绩：",a[i]);
        while (getchar()!='\n');
    }
    if (p->grade[i]>100||p->grade[i]<0)
    {
        printf ("成绩在0~100之间\n");
        goto back;
    }
    if (getchar()!='\n')
    {
        while (getchar()!='\n');
        fontColor(255,255,255);
        backgroundColor(150,100,0);
        printf ("已自动忽略非数字字符后所有内容\n");
        restoreColor();
    }
    calculateGrades(p);
}

struct student *creat()//添加数据,返回地址
{
    enter(2);
    space(50);
    fontColor(0,200,0);
    printf("添加信息");
    restoreColor();
    enter(6);
    struct student *head;
    head = (struct student *)malloc( sizeof(struct student) );
    editName(head);
    editGender(head);
    editNumber(head);
    editGrade(head,0);
    editGrade(head,1);
    editGrade(head,2);
    head->admin=0;
    head->password[0]='\0';
    head->next=NULL;
    return head;
}

struct student *fileOpen()//读取到内存
{
    FILE *fp;
    struct student *head,*p1,*p2;
    fp=fopen("student_v3.dat","rb+");
    if (fp==NULL)
    {
        //printf ("没有数据文件\n");
        return 0;
    }
    for ( int i=0 ; i==0 || p1->next!=NULL ; i++)
    {
        p1=(struct student*)calloc(1,sizeof(struct student));
        fread(p1,sizeof(struct student),1,fp);
        if (i==0) head=p1;
        if (i>0) p2->next=p1;
        p2=p1;
    }
    fclose(fp);
    return head;
}

void fileSave(struct student *head)//保存并关闭文件
{
    struct student *p;
    FILE *fp;
    p=head;
    int temp;
    if ((fp=fopen("student_v3.dat","wb+"))==NULL) printf ("fuck 保存失败？？？？\n");
    
    for (int i=0;p!=NULL;i++)
    {
        temp=fwrite(p,sizeof(struct student),1,fp);
        p=p->next;
    }
    fclose(fp);
}

int noInvaidChinese(char *p)//中文ASCII检测
{
    int i=0;
    for (;i<15;i++)
    {
        if (p[i]<0) break;
    }
    for (;i<14;)
    {
        i=i+2;
        if (i==12) return 1;
        if (i==13) return 0;
        while (p[i]>0) i++;
    }
    return 1;
}

void changeColor(int a)
{
    int r,g,b;
    switch (a)
    {
        case 10:g=255,r=0,b=0;break;
        case 9: g=255,r=60,b=0;break;
        case 8: g=255,r=180,b=0;break;
        case 7: g=255,r=250,b=0;break;
        case 6: g=120,r=255,b=0;break;
        case 5: g=0,r=255,b=0;break;
        default:g=0,r=150,b=100;break;
    }
    backgroundColor(r,g,b);
    fontColor(0,0,0);
}

void printData(struct student *p1)//打印输出
{
    if (strlen(p1->name)<8) printf ("%s\t\t",p1->name);
    else if (strlen(p1->name)<16) printf ("%s\t",p1->name);
    else if (strlen(p1->name)==16) printf ("%s",p1->name);
    else if (noInvaidChinese(p1->name)==1)
    {
        for (int i=0;i<14;i++)
        {
            printf ("%c",p1->name[i]);
        }
        printf ("…");
    }
    else
    {
        for (int i=0;i<13;i++)
        {
            printf ("%c",p1->name[i]);
        }
        printf ("… ");
    }
    if (p1->gender==1) printf(" 男\t");
    else if (p1->gender==2) printf(" 女\t");
    else printf(" ??\t");
    if (p1->number<=999999999999&&p1->number>=0) printf ("%.12lld\t",p1->number);
    else printf("????????????\t");
    int temp;
    for(int i=0;i<3;i++)
    {
        temp=p1->grade[i]/10;
        changeColor(temp);
        if (p1->grade[i]<100.0001&&p1->grade[i]>=0) printf ("%5.1f",p1->grade[i]);
        else printf("?????");
        restoreColor();
        printf ("　\t");
    }
    temp=p1->grades/10/3;
    changeColor(temp);
    if (p1->grades<300.0001&&p1->grades>=0) printf ("%5.1f",p1->grades);
    else printf("?????");
    restoreColor();
    printf ("　\n");
}

void printDataConfirm(struct student *p1)//打印输出确认
{
    printf ("姓名：%s\n性别：",p1->name);
    if (p1->gender==1) printf("男\n");
    else if (p1->gender==2) printf("女\n");
    else printf("??\n");
    printf("学号：%.12lld\n语文成绩：%.1f  数学成绩：%.1f  英语成绩：%.1f  总分：%.1f\n",p1->number,p1->grade[0],p1->grade[1],p1->grade[2],p1->grades);
}

int editData(struct student *p1)//修改数据
{
    begin:
    system ("CLS");
    printf ("请选择要修改的信息\n\n");
    printDataConfirm(p1);
    printf ("\n1.姓名 2.性别 3.学号 4.语文成绩 5.数学成绩 6.英语成绩 0.返回\n");
    printf("\n请输入 > ");
    int a;
    a=sortNumber(6);
    if (a==1) editName(p1);
    else if (a==2) editGender(p1);
    else if (a==3) editNumber(p1);
    else if (a==4) editGrade(p1,0);
    else if (a==5) editGrade(p1,1);
    else if (a==6) editGrade(p1,2);
    else if (a==0) return 1;
    fontColor(0,0,0);
    backgroundColor(0,220,0);
    printf ("修改完成\n");
    restoreColor();
    Sleep(500);
    goto begin;
}

int deleteData(struct student *p1)//删除数据
{
    system ("CLS");
    printf ("请确认要删除的信息\n");
    printDataConfirm(p1);
    printf ("\n输入'Y'确认，enter键返回\n");
    printf("\n请输入 > ");
    char a;
    scanf ("%c",&a);
    if (a=='y'||a=='Y')
    {
        while (getchar()!='\n');
    }
    else return 0;
    free(p1);
    backgroundColor(0,220,0);
    printf ("删除完成\n");
    restoreColor();
    Sleep(500);
    return 1;
}

void enterPassword(char *p)//输入密码
{
    begin:
    for (int i=0;i<64;)
    {
        p[i]=getch();
        if (p[i]=='\r'&&i==0)
        {
            printf("\n");
            printf ("请重新输入密码：");
            goto begin;
        }
        if (p[i]=='\r')
        {
            p[i]='\0';
            printf("\n");
            goto end;
        }
        if (p[i]=='\b')
        {
            if (i>0)
            {
                i--;
                printf("\b \b");
            }
            continue;
        }
        printf("*");
        i++;
    }
    fontColor(255,255,255);
    backgroundColor(255,100,0);
    printf ("\n密码长度超出限制(63字节)");
    restoreColor();
    printf ("\n\n请重新输入密码：");
    goto begin;
    end:
    restoreColor();
}

void sortNumberData(struct student *p,int n)//学号排序
{
    for (int j=1;j<n;j++)
    {
        for (int k=n-1;k>=j;k--)
        {
            struct student t;
            if ((p+k)->number<(p+k-1)->number) t=*(p+k),*(p+k)=*(p+k-1),*(p+k-1)=t;
        }
    }
}

void sortGradesData(struct student *p,int n)//成绩排序
{
    for (int j=1;j<n;j++)
    {
        for (int k=n-1;k>=j;k--)
        {
            struct student t;
            if ((p+k)->grades>(p+k-1)->grades) t=*(p+k),*(p+k)=*(p+k-1),*(p+k-1)=t;
        }
    }
}

void freeChain(struct student *head)//释放链表内存
{
    struct student *p1,*p2;
    p1=head;
    for (;p1!=NULL;)
    {
        p2=p1;
        p1=p1->next;
        free(p2);
    }
}

void freeTempChain(struct tempChain *head)//释放排序链表内存
{
    struct tempChain *p1,*p2;
    p1=head;
    for (;p1!=NULL;)
    {
        p2=p1;
        p1=p1->next;
        free(p2);
    }
}

int sortNumber(int limit)//输入序号，并返回
{
    int n;
    begin:
    for (;scanf("%d",&n)!=1;)
    {
        fontColor(255,255,255);
        backgroundColor(255,100,0);
        printf ("输入错误");
        restoreColor();
        printf ("\n\n请重新输入数字：");
        while (getchar()!='\n');
    }
    if (getchar()!='\n')
    {
        while (getchar()!='\n');
        fontColor(255,255,255);
        backgroundColor(150,100,0);
        printf ("已自动忽略非法字符后所有内容\n");
        Sleep(200);
        restoreColor();
    }
    if (n<0||n>limit)
    {
        fontColor(255,255,255);
        backgroundColor(255,100,0);
        printf ("数字范围错误");
        restoreColor();
        printf ("\n\n请重新输入序号：");
        goto begin;
    }
    return n;
}

void adminAdd()//添加管理员
{
    struct student *head,*p1;
    head=fileOpen();
    long long p;
    if (head==NULL)
    {
        menu2_4();
    }
    printf ("\n请输入要添加的管理员的学号：");
    for (;scanf("%lld",&p)!=1;)
    {
        back:
        fontColor(255,255,255);
        backgroundColor(255,100,0);
        printf ("输入错误");
        restoreColor();
        printf ("\n\n请重新输入学号：");
        while (getchar()!='\n');
    }
    if (p>999999999999||p<0)
    {
        printf ("学号为12位正整数\n");
        goto back;
    }
    if (checkNumber(p)==0)
    {
        printf ("学号不存在\n");
        goto back;
    }
    if (getchar()!='\n')
    {
        while (getchar()!='\n');
        fontColor(255,255,255);
        backgroundColor(150,100,0);
        printf ("已自动忽略非数字字符后所有内容\n");
        restoreColor();
    }
    p1=locateStudentNumber(p,head);
    p1->admin=1;
    fileSave(head);
    freeChain(head);
    menu2_4();
}

void adminOutcomeEditor(struct tempChain *p1)//管理员删除
{
    system ("CLS");
    enter(2);
    space(50);
    fontColor(255,100,20);
    printf("学生管理员\n\n\n");
    restoreColor();
    enter(2);
    printf ("序号\t姓名\t\t 性别\t学号\t\t语文\t数学\t英语\t总分\n");
    struct tempChain *head;
    head=p1;
    int j=1;
    for (;p1!=NULL;j++)
    {
        printf ("%d\t",j);
        printData(p1->p);
        p1=p1->next;
    }
    j--;
    printf("\n请输入要取消的管理员的序号(输入0取消)\n");
    printf("\n请输入选项 > ");
    int n;
    n=sortNumber(j);
    if (n==0) return;
    p1=head;
    for (int i=1;i<n;i++)
    {
        p1=p1->next;
    }
    (p1->p)->admin=0;
}

int adminOutcome(struct tempChain *p1)//管理员名单
{
    system ("CLS");
    enter(2);
    space(50);
    fontColor(255,100,20);
    printf("学生管理员\n\n\n");
    restoreColor();
    enter(2);
    printf ("序号\t姓名\t\t 性别\t学号\t\t语文\t数学\t英语\t总分\n");
    struct tempChain *head;
    head=p1;
    int j=1;
    for (;p1!=NULL;j++)
    {
        printf ("%d\t",j);
        printData(p1->p);
        p1=p1->next;
    }
    j--;
    printf("\n1.删除 2.增加 0.返回\n");
    printf("\n请输入选项 > ");
    int n;
    n=sortNumber(2);
    if (n==0) return 0;
    else if (n==1) adminOutcomeEditor(head);
    else if (n==2) return 2;
    return 1;
}

void searchOutcome(struct tempChain *p1)//处理搜索结果
{
    printf ("序号\t姓名\t\t 性别\t学号\t\t语文\t数学\t英语\t总分\n");
    struct tempChain *head;
    head=p1;
    int j=1;
    for (;p1!=NULL;j++)
    {
        printf ("%d\t",j);
        printData(p1->p);
        p1=p1->next;
    }
    j--;
    if (teacherYes==0)
    {
        system("pause");
        return;
    }
    printf("\n请输入要修改的项的序号(输入0取消)\n");
    printf("\n请输入选项 > ");
    int n;
    n=sortNumber(j);
    if (n==0) goto skip;
    p1=head;
    for (int i=1;i<n;i++)
    {
        p1=p1->next;
    }
    editData(p1->p);
    skip:;
}

int searchName(char *p)//搜索姓名
{
    struct student *head,*p1;
    struct tempChain *accordHead,*p2,*temp;
    system ("CLS");
    p1=head=fileOpen();
    if (head==NULL)
    {
        printf ("数据为空\n");
        Sleep(1000);
        return 0;
    }
    printf ("搜索结果：\n");
    int i=0;
    for (;p1!=NULL;p1=p1->next)
    {
        if (strcmp(p,p1->name)==0)//把符合的地址放到tempChain
        {
            p2=malloc(sizeof(struct tempChain));
            if (i>0) temp->next=p2;
            p2->p=p1;
            p2->next=NULL;
            if (i==0) accordHead=p2;
            temp=p2;
            i++;
        }
    }
    if (i==0)
    {
        printf ("无结果\n");
    }
    else searchOutcome(accordHead);
    fileSave(head);
    freeChain(head);
    freeTempChain(accordHead);
    return 1;
}

int searchNumber(long long p)//搜索学号
{
    struct student *head,*p1;
    struct tempChain *accordHead,*p2,*temp;
    system ("CLS");
    p1=head=fileOpen();
    if (head==NULL)
    {
        printf ("数据为空\n");
        return 0;
    }
    printf ("搜索结果：\n");
    int i=0;
    for (;p1!=NULL;p1=p1->next)
    {
        if (p==p1->number)//把符合的地址放到tempChain
        {
            p2=malloc(sizeof(struct tempChain));
            if (i>0) temp->next=p2;
            p2->p=p1;
            p2->next=NULL;
            if (i==0) accordHead=p2;
            temp=p2;
            i++;
        }
    }
    if (i==0)
    {
        printf ("无结果\n");
    }
    else searchOutcome(accordHead);
    fileSave(head);
    freeChain(head);
    freeTempChain(accordHead);
    return 1;
}

struct student *locateStudentNumber(long long p,struct student *head)
{
    struct student *p1;
    p1=head;
    for (;p1!=NULL;p1=p1->next)
    {
        if (p==p1->number)//把符合的地址放到tempChain
        {
            return p1;
        }
    }
    return NULL;
}

int checkNumber(long long p)//检查学号
{
    struct student *head,*p1;
    p1=head=fileOpen();
    if (head==NULL)
    {
        return 0;
    }
    int i=0;
    for (;p1!=NULL;p1=p1->next)
    {
        if (p==p1->number) i++;
    }
    fileSave(head);
    freeChain(head);
    if (i==0)
    {
        return 0;;
    }
    else return 1;
}

void fontColor(int r,int g,int b)//输入rgb颜色字体
{
    printf ("\x1b[38;2;%d;%d;%dm",r,g,b);
}

void backgroundColor(int r,int g,int b)//输入rgb颜色背景
{
    printf ("\x1b[48;2;%d;%d;%dm",r,g,b);
}

void restoreColor()//恢复颜色
{
    FILE *fp;
    fp=fopen("theme.dat","rb");
    int a[6];//存字体，背景rgb值
    if (fp==NULL)
    {
        //fclose(fp);
        //menu4_1();
        a[0]=0,a[1]=0,a[2]=0,a[3]=255,a[4]=255,a[5]=255;
    }
    else for (int i=0;i<6;i++)
    {
        fread(&a[i],sizeof(int),1,fp);
        if (a[i]>255) a[i]=255;
        if (a[i]<0) a[i]=0;
    }
    fclose(fp);
    fontColor(a[0],a[1],a[2]);
    backgroundColor(a[3],a[4],a[5]);
}

void storageColor(int *a)
{
    FILE *fp;
    fp=fopen("theme.dat","wb+");
    fwrite(a,6*sizeof(int),1,fp);
    fclose(fp);
}

void storageFont(wchar_t *p)
{
    FILE *fp;
    fp=fopen("font.dat","wb+");
    fwrite(p,6*sizeof(wchar_t),1,fp);
    fclose(fp);
}

void readFont()
{
    FILE *fp;
    fp=fopen("font.dat","rb");
    wchar_t b[6];//存字体
    if (fp==NULL)
    {
        wcscpy(b,L"黑体");
    }
    else fread(b,6*sizeof(wchar_t),1,fp);
    fclose(fp);
    font(b);
}

void font(wchar_t *p)//字体
{
    setlocale(LC_ALL, "zh_CN.GBK");
	HANDLE hOut = CreateFileW(L"CONOUT$", GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	CONSOLE_FONT_INFOEX cfi;
	ZeroMemory(&cfi, sizeof(cfi));
	cfi.cbSize = sizeof(cfi);
	COORD sz;
    WINBOOL fuck;
	sz.X = 12;
	sz.Y = 24;
	cfi.dwFontSize = sz;
	cfi.FontWeight = 400;
	lstrcpyW(cfi.FaceName, p);
	SetCurrentConsoleFontEx(hOut, fuck, &cfi);
}

void menu1_1()//输入顺序查看
{
    system("title 学生管理系统-查看信息-按输入顺序查看");
    struct student *head,*p1,*p2;
    system("CLS");
    enter(2);
    space(50);
    fontColor(100,100,20);
    printf("按输入顺序查看");
    restoreColor();
    enter(3);
    head=fileOpen();
    p1=head;
    if (head==NULL) printf ("\n没读到数据啊\n\n");
    else printf ("姓名\t\t 性别\t学号\t\t语文\t数学\t英语\t总分\n");
    for (;p1!=NULL;)
    {
        printData(p1);
        p1=p1->next;
    }
    freeChain(head);
    printf("\nenter键退出");
    getchar();
    menu1();
}

void menu1_2()//按学号查看
{
    system("title 学生管理系统-查看信息-按学号顺序查看");
    struct student *head,*p1,*p2,*p;
    system("CLS");
    enter(2);
    space(50);
    fontColor(100,100,20);
    printf("按学号查看");
    restoreColor();
    enter(3);
    head=fileOpen();
    int n=0;
    if (head==NULL) printf ("\n没读到数据啊\n\n");
    else printf ("姓名\t\t 性别\t学号\t\t语文\t数学\t英语\t总分\n");
    for (p1=head;p1!=NULL;)
    {
        p1=p1->next;
        n++;
    }
    p = calloc(n,sizeof(struct student));
    p2=head;
    for (int i=0;i<n;i++)
    {
        *(p+i)=*(p2);
        p2=p2->next;
    }
    sortNumberData(p,n);
    for (int i=0;i<n;i++)
    {
        printData(p+i);
    }
    free(p);
    freeChain(head);
    printf("\nenter键退出");
    getchar();
    menu1();
}

void menu1_3()//按成绩查看
{
    system("title 学生管理系统-查看信息-按成绩排名查看");
    struct student *head,*p1,*p2,*p;
    system("CLS");
    enter(2);
    space(50);
    fontColor(100,100,20);
    printf("按成绩查看");
    restoreColor();
    enter(3);
    head=fileOpen();
    int n=0;
    if (head==NULL) printf ("\n没读到数据啊\n\n");
    else printf ("姓名\t\t 性别\t学号\t\t语文\t数学\t英语\t总分\n");
    for (p1=head;p1!=NULL;)
    {
        p1=p1->next;
        n++;
    }
    p = calloc(n,sizeof(struct student));
    p2=head;
    for (int i=0;i<n;i++)
    {
        *(p+i)=*(p2);
        p2=p2->next;
    }
    sortGradesData(p,n);
    for (int i=0;i<n;i++)
    {
        printData(p+i);
    }
    free(p);
    freeChain(head);
    printf("\nenter键退出");
    getchar();
    menu1();
}

void menu1()//查信息
{
    begin: system("title 学生管理系统-查看信息");
    system("CLS");
    enter(2);
    space(50);
    fontColor(100,100,20);
    printf("学生信息查看");
    restoreColor();
    enter(6);
    space(20);
    printf("1.按照输入顺序查看\n");
    enter(1);
    space(20);
    printf("2.按学号从小到大查看\n");
    enter(1);
    space(20);
    printf("3.按成绩排名查看\n");
    enter(1);
    space(20);
    printf("0.返回主菜单\n");
    enter(3);
    printf("请输入选项 > ");
    int a,b;
    scanf ("%d",&a);
    if ((b=getchar())!='\n') goto wrong;
    if (a==1) menu1_1();
    else if (a==2) menu1_2();
    else if (a==3) menu1_3();
    else if (a==0) mainMenu();
    wrong:
    fontColor(255,255,255);
    backgroundColor(255,0,0);
    printf ("你输错了吧？");
    restoreColor();
    if (b=='\n') goto skip;
    while (getchar()!='\n');
    skip: 
    Sleep(500);
    goto begin;
}

void menu2_1()//添加
{
    system("title 学生管理系统-编辑信息-添加");
    struct student *head,*p1;
    int i;
    head=fileOpen();
    system("CLS");
    if (head==0)
    {
        head=creat();
        goto skip;
    }
    for (p1=head,i=0;p1->next!=NULL;p1=p1->next,i++);
    p1->next=creat();
    skip: 
    fileSave(head);
    freeChain(head);
    printf ("是否继续添加?\n输入'Y'继续,enter退出\n");
    printf ("\n请输入 > ");
    char a=getchar();
    if (a=='y'||a=='Y')
    {
        while (getchar()!='\n');
        menu2_1();
    }
    menu2();
}

void menu2_2()//修改
{
    system("title 学生管理系统-编辑信息-修改");
    struct student *head,*p1,*p2;
    begin:
    head=fileOpen();
    p2=p1=head;
    system("CLS");
    enter(2);
    space(50);
    fontColor(100,100,20);
    printf("学生信息修改\n\n\n");
    restoreColor();
    if (head==NULL) printf ("没读到数据啊");
    else printf ("序号\t姓名\t\t 性别\t学号\t\t语文\t数学\t英语\t总分\n");
    int a=1;
    for (;p1!=NULL;a++)
    {
        printf ("%d\t",a);
        printData(p1);
        p1=p1->next;
    }
    a--;
    printf("\n请输入要修改的项的序号(输入0取消)\n");
    printf("\n请输入选项 > ");
    int n;
    n=sortNumber(a);
    if (n==0) goto skip;
    for (int i=1;i<n;i++)
    {
        p2=p2->next;
    }
    int temp;
    temp = editData(p2);
    fileSave(head);
    freeChain(head);
    goto begin;
    skip: menu2();
}

void menu2_3()//删除
{
    system("title 学生管理系统-编辑信息-删除");
    struct student *head,*p1,*p2,*p3,*p4;
    begin:
    head=fileOpen();
    p2=p1=head;
    system("CLS");
    enter(2);
    space(50);
    fontColor(255,100,20);
    printf("学生信息删除\n\n\n");
    restoreColor();
    if (head==NULL) printf ("没读到数据啊");
    else printf ("序号\t姓名\t\t 性别\t学号\t\t语文\t数学\t英语\t总分\n");
    int a=1;
    for (;p1!=NULL;a++)
    {
        printf ("%d\t",a);
        printData(p1);
        p1=p1->next;
    }
    a--;
    printf("\n请输入要删除的项的序号(输入0取消)\n");
    printf("\n请输入选项 > ");
    int n;
    n=sortNumber(a);
    if (n==0) goto skip;
    for (int i=1;i<n-1;i++)
    {
        p2=p2->next;
    }
    int temp;
    if (n>1) p3=p2->next;
    else p3=head;
    p4=p3->next;
    temp = deleteData(p3);
    if (n==1) head=p4;
    else p2->next=p4;
    fileSave(head);
    freeChain(head);
    if (a==1) remove("student_v3.dat");
    goto begin;
    skip: menu2();
}

void menu2_4()//设置学生管理员
{
    struct student *head,*p1;
    struct tempChain *accordHead,*p2,*temp;
    system("title 学生管理系统-编辑信息-管理员");
    begin:
    system ("CLS");
    enter(2);
    space(50);
    fontColor(255,100,20);
    printf("学生管理员\n\n\n");
    restoreColor();
    int n;
    p1=head=fileOpen();
    if (head==NULL) printf ("数据为空\n");
    printf ("搜索结果：\n");
    int i=0;
    for (;p1!=NULL;p1=p1->next)
    {
        if (p1->admin)//把符合的地址放到tempChain
        {
            p2=malloc(sizeof(struct tempChain));
            if (i>0) temp->next=p2;
            p2->p=p1;
            p2->next=NULL;
            if (i==0) accordHead=p2;
            temp=p2;
            i++;
        }
    }
    if (i==0)
    {
        printf ("无管理员\n");
        printf("\n1.删除 2.增加 0.返回\n");
        printf("\n请输入选项 > ");
        n=sortNumber(2);
        if (n==0) menu2();
        else if (n==1)
        {
            printf("不可用");
            Sleep(1000);
            goto begin;
        }
        else if (n==2) adminAdd();
    }
    else n=adminOutcome(accordHead);
    fileSave(head);
    freeChain(head);
    freeTempChain(accordHead);
    if (n==1) goto begin;
    else if (n==2) adminAdd();
    else menu2();
}

void menu2()//编辑
{
    begin: system("title 学生管理系统-编辑信息");
    system("CLS");
    enter(2);
    space(50);
    fontColor(0,100,100);
    printf("学生信息修改");
    restoreColor();
    enter(6);
    space(20);
    printf("1.添加\n");
    enter(1);
    space(20);
    printf("2.修改\n");
    enter(1);
    space(20);
    printf("3.删除\n");
    enter(1);
    space(20);
    printf("4.设置管理员\n");
    enter(1);
    space(20);
    printf("0.返回主菜单\n");
    enter(1);
    printf("请输入选项 > ");
    int a,b;
    scanf ("%d",&a);
    if ((b=getchar())!='\n') goto wrong;
    if (a==1) menu2_1();
    else if (a==2) menu2_2();
    else if (a==3) menu2_3();
    else if (a==4) menu2_4();
    else if (a==0) mainMenu();
    wrong:
    fontColor(255,255,255);
    backgroundColor(255,0,0);
    printf ("你输错了吧？");
    restoreColor();
    if (b=='\n') goto skip;
    while (getchar()!='\n');
    skip: 
    Sleep(500);
    goto begin;
}

void menu3_1()//搜索姓名
{
    system("title 学生管理系统-搜索信息-姓名搜索");
    system("CLS");
    enter(2);
    space(50);
    fontColor(255,100,20);
    printf("姓名搜索\n\n\n");
    restoreColor();
    enter(5);
    char name[30];
    printf("请输入姓名 > ");
    begin:
    for (int i=0;i<30;i++)
    {
        name[i]=getchar();
        if (name[i]=='\n'&&i==0)
        {
            printf ("请重新输入姓名：");
            goto begin;
        }
        if (name[i]=='\n')
        {
            name[i]='\0';
            goto next;
        }
    }
    fontColor(255,255,255);
    backgroundColor(255,100,0);
    printf ("名字长度超出限制(29字节)");
    backgroundColor(255,255,255);//恢复颜色
    fontColor(0,0,0);
    printf ("\n\n请重新输入姓名：");
    while (getchar()!='\n');
    goto begin;
    int n;
    next:
    n=searchName(name);
    if (n==1)
    {
        printf ("\n是否继续搜索姓名?\n输入'Y'继续,enter取消\n");
        printf ("\n请输入 > ");
    }
    else menu3();
    char a=getchar();
    if (a=='y'||a=='Y')
    {
        while (getchar()!='\n');
        menu3_1();
    }
    menu3();
}

void menu3_2()//搜索学号
{
    system("title 学生管理系统-搜索信息-学号搜索");
    system("CLS");
    enter(2);
    space(50);
    fontColor(255,100,20);
    printf("学号搜索\n\n\n");
    restoreColor();
    enter(5);
    long long number;
    printf("请输入学号 > ");
    for (;scanf("%lld",&number)!=1;)
    {
        back:
        fontColor(255,255,255);
        backgroundColor(255,100,0);
        printf ("输入错误");
        restoreColor();
        printf ("\n\n请重新输入学号：");
        while (getchar()!='\n');
    }
    if (number>999999999999||number<0)
    {
        printf ("学号为12位正整数\n");
        goto back;
    }
    if (getchar()!='\n')
    {
        while (getchar()!='\n');
        fontColor(255,255,255);
        backgroundColor(150,100,0);
        printf ("已自动忽略非数字字符后所有内容\n");
        restoreColor();
    }
    int n;
    next:
    n=searchNumber(number);
    if (n==1)
    {
        printf ("\n是否继续搜索学号?\n输入'Y'继续,enter取消\n");
        printf ("\n请输入 > ");
    }
    else menu3();
    char a=getchar();
    if (a=='y'||a=='Y')
    {
        while (getchar()!='\n');
        menu3_2();
    }
    menu3();
}

void menu3()//搜索
{
    begin: system("title 学生管理系统-搜索信息");
    system("CLS");
    enter(2);
    space(50);
    fontColor(0,100,100);
    printf("学生信息搜索");
    restoreColor();
    enter(6);
    space(20);
    printf("1.搜索姓名\n");
    enter(1);
    space(20);
    printf("2.搜索学号\n");
    enter(1);
    space(20);
    printf("0.返回主菜单\n");
    enter(5);
    printf("请输入选项 > ");
    int a,b;
    scanf ("%d",&a);
    if ((b=getchar())!='\n') goto wrong;
    if (a==1) menu3_1();
    else if (a==2) menu3_2();
    else if (a==0) mainMenu();
    wrong:
    fontColor(255,255,255);
    backgroundColor(255,0,0);
    printf ("你输错了吧？");
    restoreColor();
    if (b=='\n') goto skip;
    while (getchar()!='\n');
    skip: 
    Sleep(500);
    goto begin;
}

void menu4_1()//改颜色
{
    begin:
    system("title 学生管理系统-主题设置-颜色设置");
    system("CLS");
    enter(2);
    space(50);
    printf("颜色设置");
    enter(6);
    fontColor(255,255,255);
    backgroundColor(0,0,0);
    printf("1.黑底白字\n\n");
    fontColor(0,0,0);
    backgroundColor(255,255,255);
    printf("2.白底黑字\n\n");
    backgroundColor(238,124,9);
    fontColor(0,0,0);
    printf("3.经典橙底黑字\n\n");
    backgroundColor(0,158,47);
    fontColor(113,255,30);
    printf("4.护眼原谅色\n\n");
    backgroundColor(255, 150, 251);
    fontColor(197, 34, 225);
    printf("5.甜甜少女心\n");
    fontColor(255,255,255);
    backgroundColor(0,0,0);
    enter(1);
    printf("请输入选项 > ");
    int a,color[6];
    char b;
    scanf ("%d",&a);
    if ((b=getchar())!='\n') goto wrong;
    if (a==1)
    {
        color[0]=255,color[1]=255,color[2]=255,color[3]=0,color[4]=0,color[5]=0;
        goto end;
    }
    else if (a==2)
    {
        color[0]=0,color[1]=0,color[2]=0,color[3]=255,color[4]=255,color[5]=255;
        goto end;
    }
    else if (a==3)
    {
        color[0]=0,color[1]=0,color[2]=0,color[3]=238,color[4]=124,color[5]=9;
        goto end;
    }
    else if (a==4)
    {
        color[0]=113,color[1]=255,color[2]=30,color[3]=0,color[4]=158,color[5]=47;
        goto end;
    }
    else if (a==5)
    {
        color[0]=197,color[1]=34,color[2]=225,color[3]=255,color[4]=150,color[5]=251;
        goto end;
    }
    wrong:
    printf ("你输错了吧？");
    if (b=='\n') goto skip;
    while (getchar()!='\n');
    skip: 
    Sleep(500);
    goto begin;
    end:
    storageColor(color);
    //system("color 0f");
    restoreColor();
    mainMenu();
}

void menu4_2()//改字体
{
    begin:
    system("title 学生管理系统-主题设置-字体设置");
    system("CLS");
    enter(2);
    space(50);
    printf("字体设置");
    enter(6);
    printf("1.黑体\n\n");
    printf("2.楷体\n\n");
    printf("3.新宋体\n\n");
    printf("4.幼圆\n");
    enter(3);
    printf("请输入选项 > ");
    int a,b;
    wchar_t font[6];
    scanf ("%d",&a);
    if ((b=getchar())!='\n') goto wrong;
    if (a==1)
    {
        wcscpy(font,L"黑体");
        goto end;
    }
    else if (a==2)
    {
        wcscpy(font,L"楷体");
        goto end;
    }
    else if (a==3)
    {
        wcscpy(font,L"新宋体");
        goto end;
    }
    else if (a==4)
    {
        wcscpy(font,L"幼圆");
        goto end;
    }
    wrong:
    fontColor(255,255,255);
    backgroundColor(255,0,0);
    printf ("你输错了吧？");
    restoreColor();
    if (b=='\n') goto skip;
    while (getchar()!='\n');
    skip: 
    Sleep(500);
    goto begin;
    end:
    storageFont(font);
    readFont();
    mainMenu();
}

void menu4_3()
{
    system("title 学生管理系统-主题设置-高级色彩设置");
    system("CLS");
    enter(2);
    space(50);
    printf("高级颜色设置");
    enter(6);
    printf("此功能仅面向高级用户，使用RGB的值进行调整\n如果随意调整可能会出现字体看不清的情况\n如果出现，需要手动到程序目录中删掉theme.dat文件\n继续请输入'Y'，其他键退出\n\n");
    printf("请输入 > ");
    char a;
    a=getchar();
    while (getchar()!='\n');
    if (a=='y'||a=='Y');
    else menu4();
    int color[6];
    back:
    system("CLS");
    printf("请输入字体的R值(0-255):");
    color[0]=sortNumber(255);
    printf("请输入字体的G值(0-255):");
    color[1]=sortNumber(255);
    printf("请输入字体的B值(0-255):");
    color[2]=sortNumber(255);
    printf("请输入背景的R值(0-255):");
    color[3]=sortNumber(255);
    printf("请输入背景的G值(0-255):");
    color[4]=sortNumber(255);
    printf("请输入背景的B值(0-255):");
    color[5]=sortNumber(255);
    printf("\n以下为颜色预览，能看清请输入'Y'，输入'R'重新输入，其它键放弃\n");
    fontColor(color[0],color[1],color[2]);
    backgroundColor(color[3],color[4],color[5]);
    printf("你调这个设置真是闲得蛋疼");
    restoreColor();
    printf("\n\n请输入 > ");
    char b;
    b=getchar();
    while (getchar()!='\n');
    if (b=='y'||b=='Y')
    {
        if ((abs(color[0]-color[3])+abs(color[1]-color[4])+abs(color[2]-color[5]))<=40)
        {
            printf("系统已阻止智障操作，你是写轮眼吗？\n2秒后请重新输入");
            Sleep(2000);
            goto back;
        }
        storageColor(color);
        restoreColor();
    }
    else if (b=='r'||b=='R') goto back;
    menu4();
}

void menu4()//主题修改
{
    begin: system("title 学生管理系统-主题设置");
    system("CLS");
    enter(2);
    space(50);
    fontColor(0,100,100);
    printf("个性化设定");
    restoreColor();
    enter(6);
    space(20);
    printf("1.颜色设置\n");
    enter(1);
    space(20);
    printf("2.字体设置\n");
    enter(1);
    space(20);
    printf("3.高级颜色设置\n");
    enter(1);
    space(20);
    printf("0.返回主菜单");
    enter(5);
    printf("请输入选项 > ");
    int a,b;
    scanf ("%d",&a);
    if ((b=getchar())!='\n') goto wrong;
    if (a==1) menu4_1();
    else if (a==2) menu4_2();
    else if (a==3) menu4_3();
    else if (a==0) mainMenu();
    wrong:
    fontColor(255,255,255);
    backgroundColor(255,0,0);
    printf ("你输错了吧？");
    restoreColor();
    if (b=='\n') goto skip;
    while (getchar()!='\n');
    skip: 
    Sleep(500);
    goto begin;
}

void mainMenu()//主菜单
{
    begin: system("title 学生管理系统(ver 0.6.1 stable)");
    readFont();
    system("CLS");
    enter(2);
    space(50);
    fontColor(255,255,255);
    backgroundColor(0,0,0);
    printf("学生管理");
    backgroundColor(238,124,9);
    fontColor(0,0,0);
    printf ("系统");
    restoreColor();
    if (teacherYes) printf("(教师登录)");
    else printf("(学生登录)");
    enter(6);
    space(20);
    printf("1.查看学生信息\n");
    enter(1);
    space(20);
    printf("2.编辑学生信息\n");
    enter(1);
    space(20);
    printf("3.搜索学生信息\n");
    enter(1);
    space(20);
    printf("4.主题设置\n");
    enter(1);
    space(20);
    printf("0.退出登录\n");
    enter(1);
    printf("请输入选项 > ");
    int a;
    char b;
    scanf ("%d",&a);
    if ((b=getchar())!='\n') goto wrong;
    if (a==1) menu1();
    else if (a==2)
    {
        if (teacherYes) menu2();
        else
        {
            fontColor(255,255,255);
            backgroundColor(255,0,0);
            printf ("当前用户没有权限");
            restoreColor();
            goto retry;
        }
    }
    else if (a==3) menu3();
    else if (a==4) menu4();
    else if (a==0)
    {
        teacherYes=0;
        welcomeMenu();
    }
    wrong:
    fontColor(255,255,255);
    backgroundColor(255,0,0);
    printf ("你输错了吧？");
    restoreColor();
    retry:
    if (b=='\n') goto skip;
    while (getchar()!='\n');
    skip: 
    Sleep(500);
    goto begin;
}

void setPassWord()//设置密码
{
    FILE *fp,*fptest;
    system("CLS");
    char b[64],input[64],new[64];
    fptest=fopen("password.dat","rb+");
    if (fptest==NULL)
    {
        printf("您没有设置教师密码，请设置\n");
        goto skip;
    }
    else fread(b,64*sizeof(char),1,fptest);
    printf("请输入目前的密码：");
    back:
    fontColor(0,0,0);backgroundColor(0,0,0);
    enterPassword(input);
    restoreColor();
    if (strcmp(input,b)==0)
    {
        teacherYes=1;
    }
    else
    {
        printf("密码错误\n请重新输入密码：");
        goto back;
    }
    skip:
    printf("请设置密码：");
    enterPassword(new);
    fp=fopen("password.dat","wb+");
    fwrite(new,64*sizeof(char),1,fp);
    fclose(fp);
    fontColor(0,0,0);
    backgroundColor(0,220,0);
    printf ("修改完成\n");
    restoreColor();
    Sleep(500);
    teacherYes=1;
    welcomeMenu();
}

void teacherLoginMenu()//教师登录界面
{
    FILE *fp;
    system("CLS");
    fp=fopen("password.dat","rb");
    char b[64],input[64];
    enter(6);
    if (fp==NULL)
    {
        fclose(fp);
        setPassWord();
        goto end;
    }
    else fread(b,64*sizeof(char),1,fp);
    fclose(fp);
    printf("请输入密码：");
    back:
    enterPassword(input);
    if (strcmp(input,b)==0)
    {
        teacherYes=1;
    }
    else
    {
        printf("密码错误\n请重新输入密码：");
        goto back;
    }
    end:;
}

long long enterStudentNumber()//输入检查学号
{
    long long num;
    for (;scanf("%lld",&num)!=1;)
    {
        back:
        while (getchar()!='\n');
        fontColor(255,255,255);
        backgroundColor(255,100,0);
        printf ("输入错误");
        restoreColor();
        printf ("\n是否继续尝试?\n输入'Y'继续,enter返回\n");
        printf ("\n请输入 > ");
        char a=getchar();
        if (a=='y'||a=='Y')
        {
            while (getchar()!='\n');
        }
        else studentLoginMenu();
        printf ("\n\n请重新输入学号：");
        
    }
    if (num>999999999999||num<0)
    {
        printf ("学号为12位正整数\n");
        goto back;
    }
    if (checkNumber(num)==0)
    {
        printf ("学号不存在\n");
        goto back;
    }
    if (getchar()!='\n')
    {
        while (getchar()!='\n');
        fontColor(255,255,255);
        backgroundColor(150,100,0);
        printf ("已自动忽略非数字字符后所有内容\n");
        restoreColor();
    }
    return num;
}

void setStudentPassword(struct student *p1)
{
    if (strlen(p1->password)==0) printf("您还没有设置密码\n");
    printf("请设置密码：");
    enterPassword(p1->password);
    fontColor(0,0,0);
    backgroundColor(0,220,0);
    printf ("修改完成\n");
    restoreColor();
    Sleep(500);

}

void studentLogin(int n)//学生登录界面
{
    system("CLS");
    enter(2);
    space(50);
    printf("学生登录\n");
    enter(5);
    printf("请输入您的学号：");
    long long num=enterStudentNumber();
    struct student *head,*p1;
    head=fileOpen();
    p1=locateStudentNumber(num,head);
    if (strlen(p1->password)==0)
    {
        setStudentPassword(p1);
        fileSave(head);
        goto skip;
    }
    printf("请输入密码：");
    char input[64];
    back:
    enterPassword(input);
    if (strcmp(input,p1->password)==0)
    {
        if (n==3)
        {
            setStudentPassword(p1);
            fileSave(head);
            freeChain(head);
        }
        skip:
        if (n==1)
        {
            system("CLS");
            enter(4);
            printDataConfirm(p1);
            printf("\n");
            system("pause");
            freeChain(head);
            studentLoginMenu();
        }
        if (n==2&&p1->admin==1)
        {
            freeChain(head);
            mainMenu();
        }
        if (n==2&&p1->admin!=1)
        {
            printf("您没有权限查看他人成绩，2秒后自动进入个人成绩查看界面");
            Sleep(2000);
            system("CLS");
            enter(4);
            printDataConfirm(p1);
            printf("\n");
            freeChain(head);
            system("pause");
            studentLoginMenu();
        }
        studentLoginMenu();
    }
    else
    {
        printf("密码错误\n请重新输入密码：");
        goto back;
    }
}

void studentLoginMenu()//学生登录菜单
{
    begin:
    system("CLS");
    enter(2);
    space(50);
    printf("学生登录\n");
    enter(5);
    space(20);
    printf("1.普通学生登录\n");
    enter(1);
    space(20);
    printf("2.管理员学生登录\n");
    enter(1);
    space(20);
    printf("3.修改学生登录密码\n");
    enter(1);
    space(20);
    printf("0.返回登录界面\n");
    enter(3);
    printf("请输入选项 > ");
    int a,b;
    scanf ("%d",&a);
    if ((b=getchar())!='\n') goto wrong;
    if (a==1) studentLogin(1);
    else if (a==2) studentLogin(2);
    else if (a==3) studentLogin(3);
    else if (a==0) welcomeMenu();
    wrong:
    fontColor(255,255,255);
    backgroundColor(255,0,0);
    printf ("你输错了吧？");
    restoreColor();
    if (b=='\n') goto skip;
    while (getchar()!='\n');
    skip: 
    Sleep(500);
    goto begin;
}

void welcomeMenu()//
{
    begin: system("title 学生管理系统登录");
    system("CLS");
    enter(2);
    space(46);
    fontColor(0,100,100);
    printf("欢迎使用学生管理系统");
    restoreColor();
    printf("\n                                                  \x1b[7m made by LNS \x1b[0m");
    restoreColor();
    printf("　");
    enter(5);
    space(20);
    printf("1.教师登录\n");
    enter(1);
    space(20);
    printf("2.学生登录\n");
    enter(1);
    space(20);
    printf("3.修改教师密码\n");
    enter(1);
    space(20);
    printf("0.退出系统\n");
    enter(3);
    printf("请输入选项 > ");
    int a,b;
    scanf ("%d",&a);
    if ((b=getchar())!='\n') goto wrong;
    if (a==1) 
    {
        teacherLoginMenu();
        mainMenu();
    }
    else if (a==2) studentLoginMenu();
    else if (a==3)
    {
        setPassWord();
        mainMenu();
    }
    else if (a==0) QRcode();
    wrong:
    fontColor(255,255,255);
    backgroundColor(255,0,0);
    printf ("你输错了吧？");
    restoreColor();
    if (b=='\n') goto skip;
    while (getchar()!='\n');
    skip: 
    Sleep(500);
    goto begin;
}

void space(int a)//打空格
{
    for (int i=0;i<a;i++)
    {
        printf (" ");
    }
}

void enter(int a)//打回车
{
    for (int i=0;i<a;i++)
    {
        printf ("\n");
    }
}

void QRcode()
{
    system("CLS");
    /*setlocale(LC_ALL, "zh_CN.GBK");
	HANDLE hOut = CreateFileW(L"CONOUT$", GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	CONSOLE_FONT_INFOEX cfi;
	ZeroMemory(&cfi, sizeof(cfi));
	cfi.cbSize = sizeof(cfi);
	COORD sz;
    WINBOOL fuck;
	sz.X = 10;
	sz.Y = 20;
	cfi.dwFontSize = sz;
	cfi.FontWeight = 400;
	lstrcpyW(cfi.FaceName, L"黑体");
	SetCurrentConsoleFontEx(hOut, fuck, &cfi);*/
    printf("\n　█ █ █ █ █ █ █ 　　　　█ █ █ █ 　　█ 　　█ 　█ █ █ █ █ █ █ \n\
　█ 　　　　　█ 　　█ █ 　　　　　　　█ 　█ 　█ 　　　　　█ \n\
　█ 　█ █ █ 　█ 　　　　　█ 　█ 　█ 　　█ 　　█ 　█ █ █ 　█ \n\
　█ 　█ █ █ 　█ 　█ 　　　█ █ 　█ █ █ █ 　　　█ 　█ █ █ 　█ \n\
　█ 　█ █ █ 　█ 　█ █ 　　　　　　█ 　　█ █ 　█ 　█ █ █ 　█ \n\
　█ 　　　　　█ 　　　　█ █ █ █ 　█ 　█ 　　　█ 　　　　　█ \n\
　█ █ █ █ █ █ █ 　█ 　█ 　█ 　█ 　█ 　█ 　█ 　█ █ █ █ █ █ █ \n\
　　　　　　　　　　　█ █ 　　█ 　　　█ █ █ 　　　　　　　　\n\
　█ █ 　　　█ █ █ 　█ 　█ 　█ █ █ 　█ █ █ 　　　　█ █ 　　　\n\
　　█ 　　█ 　　█ █ █ 　　█ 　█ █ 　　　█ 　█ 　█ █ 　█ █ 　\n\
　　█ 　█ █ 　█ █ 　　█ 　　　█ █ █ █ █ 　　　　　█ 　　　　\n\
　█ 　█ 　　█ 　　█ █ 　　█ 　█ 　█ 　　　█ █ 　　　█ 　　　\n\
　　█ 　█ █ █ █ █ █ █ 　█ █ 　　　█ 　█ 　█ 　█ █ 　　　　█ \n\
　　█ 　　█ 　　█ 　　　　　　█ 　█ █ █ █ █ 　█ █ █ 　　█ █ \n\
　　　　　　█ █ █ █ █ 　█ █ █ 　█ 　█ 　　█ █ 　　█ █ █ 　　\n\
　　█ █ 　　█ 　█ 　　█ █ 　　█ 　　　　█ 　　　█ █ 　█ 　█                 ←查看源代码请扫码或访问下方地址\n\
　　　█ █ █ 　█ █ █ █ █ █ 　　　█ █ 　　█ 　█ 　█ 　█ █ 　　                         ↓↓↓↓↓\n\
　█ █ 　█ █ 　　█ 　█ █ 　█ █ 　█ █ 　　█ 　　█ █ █ 　█ █ █ \n\
　█ █ 　█ █ 　█ █ █ █ █ █ 　　　█ █ 　█ █ 　　　　█ █ 　　█ \n\
　█ 　█ █ 　█ 　　█ █ 　　█ █ 　█ █ 　　　█ 　█ 　　　　　　\n\
　█ 　　█ █ 　█ 　　　█ 　█ 　█ 　　█ 　█ █ █ █ █ █ 　█ █ █ \n\
　　　　　　　　　█ █ 　　　　█ █ 　　　　█ 　　　█ █ 　　　          https://github.com/lns103/StudentManager\n\
　█ █ █ █ █ █ █ 　█ █ 　█ █ 　　█ █ 　　█ █ 　█ 　█ █ █ 　　\n\
　█ 　　　　　█ 　█ 　█ █ 　　█ 　█ 　█ █ █ 　　　█ 　　　　\n\
　█ 　█ █ █ 　█ 　　█ █ 　　█ █ █ █ 　　█ █ █ █ █ █ █ 　█ 　\n\
　█ 　█ █ █ 　█ 　　█ 　　█ █ █ █ █ 　　█ █ █ 　　　█ █ 　█ \n\
　█ 　█ █ █ 　█ 　　█ █ 　　█ █ █ 　　　█ █ █ █ █ █ █ █ █ 　\n\
　█ 　　　　　█ 　█ █ 　　█ █ █ 　　　　　　　█ █ 　█ █ 　█ \n\
　█ █ █ █ █ █ █ 　█ █ █ 　█ 　█ █ 　█ 　█ 　　█ █ █ 　█ 　　\n");
    system("pause");
}

int main()
{
    readFont();
    system("color 0f");
    restoreColor();
    welcomeMenu();
}