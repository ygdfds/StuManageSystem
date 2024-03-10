#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define HIGH 25//菜单高度
#define WIDTH 103//菜单宽度

static int page = 1;//记录正在进行操作的是哪一页

typedef enum { false, true }bool;//布尔值

struct Student//用于存储学生信息
{
    char num[15];//学号
    char name[20];//姓名
    char class[30];//班级
    char phone[15];//号码
    char addr[30];//地址
};

typedef struct ListNode
{
    int number;
    struct Student stu;
    struct ListNode* prior, * next;

    void (*Insert)(struct ListNode* head, const char* num, const char* name, const char* class, const char* phone, const char* addr, int number);
    void (*Delete)(struct ListNode* head, int number);
    void (*Sort)(struct ListNode* head, struct ListNode* low, struct ListNode* high);
    struct ListNode* (*Search)(struct ListNode* head, int number);
    int (*is_exist)(struct ListNode* head, const char* num);
    int (*search_number)(struct ListNode* head, char* buffer);
    int (*Length)(struct ListNode* head);
    void (*savefile)(struct ListNode* head);
    void (*readfile)(struct ListNode* head);
    int (*get_number)(struct ListNode p);
}*LinkList;

struct User//记录每个操作的坐标
{
    int x;
    int y;
};

typedef enum { NUM, NAME, CLASS, PHONE, ADDR, MODIFY, DEL, ADD, SORT, LTURN, RTURN, USER, SEARCH, QUIT }MenuData;//操作名称

struct Menu
{
    struct User mu[14];//操作坐标的数组

    void (*Operate)(struct Menu* fu, struct ListNode* head);
    void (*get_add)(struct Menu* fu, struct ListNode head);
    void (*get_user)(struct Menu* fu, struct ListNode head);
    int (*Input)(struct Menu* fu, LinkList head);
    void (*Add)(struct Menu* fu, LinkList head);
    void (*MenuDelete)(struct Menu* fu, LinkList head, int number);
    void (*Modify)(struct Menu* fu, LinkList head, int number);
    void (*Lturn)(struct Menu* fu, struct ListNode head);
    void (*Rturn)(struct Menu* fu, struct ListNode head);
    void (*MenuSearch)(struct Menu* fu, struct ListNode head);
    int (*Quit)(struct Menu* fu, struct ListNode* head);
};

//链表相关操作
void InitList(LinkList head);
void Insert(LinkList head, const char* num, const char* name, const char* class, const char* phone, const char* addr, int number);
void Delete(LinkList head, int number);
void Sort(LinkList head, struct ListNode* low, struct ListNode* high);
struct ListNode* Search(LinkList head, int number);
int is_exist(LinkList head, const char* num);
int search_number(LinkList head, char* buffer);
int Length(LinkList head);
int get_number(struct ListNode p);

//文件相关操作
void savefile(LinkList head);
void readfile(LinkList head);

//菜单对象相关操作
int Start();
void InitMenu(struct Menu* fu);
void ShowMenu(struct Menu* fu, struct ListNode head, char* target);
void Operate(struct Menu* fu, LinkList head);
void get_add(struct Menu* fu, struct ListNode head);
void get_user(struct Menu* fu, struct ListNode head);
int Input(struct Menu* fu, LinkList head);

//文字输入相关操作
bool input_string(struct Menu* fu, LinkList head, char* target, int x, MenuData f);
void ShowAdd(LinkList head, const char* target, int x);

//选项操作相关操作
void Add(struct Menu* fu, LinkList head);
void MenuDelete(struct Menu* fu, LinkList head, int number);
void Modify(struct Menu* fu, LinkList head, int number);
void Lturn(struct Menu* fu, struct ListNode head);
void Rturn(struct Menu* fu, struct ListNode head);
void MenuSearch(struct Menu* fu, struct ListNode head);
int Quit(struct Menu* fu,struct ListNode* head);

//稳定清屏
void gotoxy(int x, int y);//控制光标位置达到清屏目的
void HideCursor();//隐藏光标，使清屏的闪烁不明显

int main()
{
    HideCursor();
    system("color 0E");
    struct Menu fu;
    struct ListNode head;
    InitList(&head);
    InitMenu(&fu);
    fu.Operate(&fu, &head);
    system("cls");
    return 0;
}

void gotoxy(int x, int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取标准输出句柄
    COORD pos = { x,y };//设置控制台中的位置
    SetConsoleCursorPosition(handle, pos);//控制光标将其移至对应位置
}

void HideCursor()
{
    CONSOLE_CURSOR_INFO cursor_info = { 1,0 };//设置光标类型，0表示隐藏光标
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);//设置控制台中光标类型
}

void Swap(struct ListNode* p, struct ListNode* q)//交换两结点的信息
{
    if (p && q)
    {
        struct Student* temp = malloc(sizeof(struct Student));
        strcpy(temp->num, p->stu.num);
        strcpy(temp->name, p->stu.name);
        strcpy(temp->class, p->stu.class);
        strcpy(temp->phone, p->stu.phone);
        strcpy(temp->addr, p->stu.addr);
        strcpy(p->stu.num, q->stu.num);
        strcpy(p->stu.name, q->stu.name);
        strcpy(p->stu.class, q->stu.class);
        strcpy(p->stu.phone, q->stu.phone);
        strcpy(p->stu.addr, q->stu.addr);
        strcpy(q->stu.num, temp->num);
        strcpy(q->stu.name, temp->name);
        strcpy(q->stu.class, temp->class);
        strcpy(q->stu.phone, temp->phone);
        strcpy(q->stu.addr, temp->addr);
        free(temp);
    }
}

void InitList(LinkList head)//初始化链表
{
    head->number = 0;
    head->prior = NULL;
    head->next = NULL;
    head->Delete = Delete;
    head->Insert = Insert;
    head->Sort = Sort;
    head->is_exist = is_exist;
    head->search_number = search_number;
    head->Length = Length;
    head->Search = Search;
    head->savefile = savefile;
    head->readfile = readfile;
    head->get_number = get_number;
    head->readfile(head);
}

void Insert(LinkList head, const char* num, const char* name, const char* class, const char* phone, const char* addr, int number)//将信息插入链表
{
    struct ListNode* p = (struct ListNode*)malloc(sizeof(struct ListNode));
    strcpy(p->stu.num, num);
    strcpy(p->stu.name, name);
    strcpy(p->stu.class, class);
    strcpy(p->stu.phone, phone);
    strcpy(p->stu.addr, addr);
    p->get_number = get_number;
    p->next = NULL;
    struct ListNode* q = head;
    while (q->next != NULL) {
        if (number != 0 && q->next->number == number)
        {
            struct ListNode* t = q->next;
            q->next = p;
            p->prior = q;
            p->number = q->number + 1;
            t->prior = p;
            p->next = t;
            while (t)//在number处插入结点后，需要使该结点之后的所有结点的number值加1
            {
                t->number++;
                t = t->next;
            }
            return;
        }
        q = q->next;
    }
    q->next = p;
    p->prior = q;
    p->number = q->number + 1;
}

void Delete(LinkList head, int number)//删除链表的结点
{
    struct ListNode* p = head;
    while (p->next != NULL && p->next->number != number)
        p = p->next;
    struct ListNode* q = p->next;
    if (q->next != NULL)
        q->next->prior = p;
    p->next = q->next;
    free(q);
    p = p->next;
    while (p)//删除结点后需要使该结点之后的所有结点number值减1
    {
        p->number--;
        p = p->next;
    }
}

void Sort(LinkList head, struct ListNode* low, struct ListNode* high)//使用快速排序对链表的中的学号信息进行比较并排序
{
    struct ListNode* p = low, * q = high;
    if (low == NULL || high == NULL || low->number >= high->number)
        return;
    struct Student* temp = malloc(sizeof(struct Student));
    strcpy(temp->num, low->stu.num);
    while (p->number < q->number)
    {
        while (p->number < q->number && strncmp(q->stu.num, temp->num, 8) >= 0)
            q = q->prior;
        while (p->number < q->number && strncmp(p->stu.num, temp->num, 8) <= 0)
            p = p->next;
        if (p->number < q->number)
            Swap(p, q);
    }
    Swap(low, p);
    Sort(head, low, p->prior);
    Sort(head, p->next, high);
    free(temp);
}

struct ListNode* Search(LinkList head, int number)//搜索对应number值的结点并返回
{
    struct ListNode* p = head;
    while (p)
    {
        if (p->number == number)
            return p;
        p = p->next;
    }
    return NULL;
}

int is_exist(LinkList head, const char* num)//计算学号存在多少次，用于输入函数input_string()处
{
    if (!head->next)
        return 0;
    int count = 0;
    struct ListNode* p = head->next;
    while (p)
    {
        if (strcmp(p->stu.num, num) == 0)
            count++;
        p = p->next;
    }
    return count;
}

int search_number(LinkList head, char* buffer)//配合搜索功能使用
{
    struct ListNode* p = head->next;
    while (p)
    {
        if (strcmp(p->stu.num, buffer) == 0 || strcmp(p->stu.name, buffer) == 0)
            return p->number;
        p = p->next;
    }
    return 0;
}

int Length(LinkList head)//获取链表的长度
{
    int length = 0;
    struct ListNode* p = head->next;
    while (p)
    {
        length++;
        p = p->next;
    }
    return length;
}

void savefile(LinkList head)//文件保存
{
    FILE* fp = fopen("StuManage", "w");
    if (fp == NULL)
        return;
    LinkList p = head->next;
    while (p)
    {
        fprintf(fp, "%s\t%s\t%s\t%s\t%s\n", p->stu.num, p->stu.name, p->stu.class, p->stu.phone, p->stu.addr);
        p = p->next;
    }
    fclose(fp);
}

void readfile(struct ListNode* head)//文件读取
{
    FILE* fp = fopen("StuManage", "r");
    if (fp == NULL)
        return;
    char num[15];
    char name[20];
    char class[30];
    char phone[15];
    char addr[30];
    while (fscanf(fp, "%s\t%s\t%s\t%s\t%s\n", num, name, class, phone, addr) != EOF)
        head->Insert(head, num, name, class, phone, addr, 0);
    fclose(fp);
}

int get_number(struct ListNode p)//获取链表对象的number值
{
    return p.number;
}

void InitMenu(struct Menu* fu)//初始化菜单对象
{
    fu->mu[NUM].x = fu->mu[NAME].x = fu->mu[CLASS].x = fu->mu[PHONE].x = fu->mu[ADDR].x = fu->mu[MODIFY].x = fu->mu[DEL].x = 3;
    fu->mu[NUM].y = 5, fu->mu[NAME].y = 20, fu->mu[CLASS].y = 35, fu->mu[PHONE].y = 50, fu->mu[ADDR].y = 65, fu->mu[MODIFY].y = 80, fu->mu[DEL].y = 95;
    fu->mu[SORT].x = 2, fu->mu[SORT].y = 2;
    fu->mu[SEARCH].x = 2, fu->mu[SEARCH].y = 84;
    fu->mu[QUIT].x = HIGH - 1, fu->mu[QUIT].y = 2;
    fu->mu[LTURN].x = HIGH - 1, fu->mu[LTURN].y = WIDTH - strlen("□<-翻页->□") - 3;
    fu->mu[RTURN].x = HIGH - 1, fu->mu[RTURN].y = WIDTH - 4;

    fu->Operate = Operate;
    fu->get_add = get_add;
    fu->get_user = get_user;
    fu->Input = Input;
    fu->Add = Add;
    fu->MenuDelete = MenuDelete;
    fu->Modify = Modify;
    fu->Lturn = Lturn;
    fu->Rturn = Rturn;
    fu->MenuSearch = MenuSearch;
    fu->Quit = Quit;
}

int Start()
{
    int i;
    int j;
    for (i = 1; i <= HIGH; i++)
    {
        for (j = 1; j <= WIDTH; j++)
        {
            if (i == 1 || i == HIGH)//绘制菜单边框
            {
                if (i == 1 && j == 1)printf("╔");
                else if (i == 1 && j == WIDTH)printf("╗");
                else if (i == HIGH && j == 1)printf("╚");
                else if (i == HIGH && j == WIDTH)printf("╝");
                else
                    printf("═");
            }
            else if (j == 1 || j == WIDTH)
                printf("║");
            else if (i == 8)
            {
                switch (j)
                {
                    case 33:
                        printf("学");
                        j += 1;
                        break;
                    case 40:
                        printf("生");
                        j += 1;
                        break;
                    case 47:
                        printf("管");
                        j += 1;
                        break;
                    case 54:
                        printf("理");
                        j += 1;
                        break;
                    case 61:
                        printf("系");
                        j += 1;
                        break;
                    case 68:
                        printf("统");
                        j += 1;
                        break;
                    default:
                        printf(" ");
                        break;
                }
            }
            else if (i == 15 && j == 39)
            {
                printf(">> 点击 ENTER 进入菜单 <<");
                j += strlen(">> 点击 ENTER 进入菜单 <<") - 1;
            }
            else if (i == 17 && j == 40)
            {
                printf(">> 点击 ESC 退出程序 <<");
                j += strlen(">> 点击 ESC 退出程序 <<") - 1;
            }
            else if (i == 22 && j == 16)
            {
                printf("使用说明：菜单界面点击wsad键可以进行上下左右移动，点击ENTER键执行对应操作");
                j += strlen("使用说明：菜单界面点击wsad键可以进行上下左右移动，点击ENTER键执行对应操作") - 1;
            }
            else if (i == 23 && j == 11)
            {
                printf("在添加、查找与搜索功能时输入可以通过点击ENTER键确认最终输入结果，点击ESC键放弃输入");
                j += strlen("在添加、查找与搜索功能时输入可以通过点击ENTER键确认最终输入结果，点击ESC键放弃输入") - 1;
            }
            else
                printf(" ");
        }
        printf("\n");
    }
    char input = _getch();
    while (input != '\r')
    {
        if (input == 27)
            return 0;
        input = _getch();
    }
    return 1;
}

void ShowMenu(struct Menu* fu, struct ListNode head, char* target)//显示菜单
{
    int i;
    int j;
    int len = head.Length(&head);
    int pagesum = len / 20 + 1;//总页数
    struct ListNode* pnode = &head;//每页链表数据的头指针
    int x = (page - 1) * 20;
    for ( i = 0; i < x; i++)
        pnode = pnode->next;
    LinkList p = pnode;//为访问每个结点而创建的指针
    for (i = 1; i <= HIGH; i++)
    {
        if ((page == pagesum && i > fu->mu[NUM].x && i < fu->mu[NUM].x + len % 20 + 1) || (page < pagesum && i > fu->mu[NUM].x && i < fu->mu[NUM].x + 21))//准备打印下一行时检查该行是否在链表显示的规定区域内
            p = p->next;//若在区域内，则准备访问下一个结点的数据
        for ( j = 1; j <= WIDTH; j++)
        {
            if (i == 1 || i == HIGH)//绘制菜单边框
            {
                if (i == 1 && j == 1)printf("╔");
                else if (i == 1 && j == WIDTH)printf("╗");
                else if (i == HIGH && j == 1)printf("╚");
                else if (i == HIGH && j == WIDTH)printf("╝");
                else
                    printf("═");
            }
            else if (j == 1 || j == WIDTH)
                printf("║");
            else if (i == 3)//该行显式对应列的数据名称或者对应操作的名称
            {
                switch (j)
                {
                    case 5:j += 3;
                        printf("学号");
                        break;
                    case 20:j += 3;
                        printf("姓名");
                        break;
                    case 35:j += 3;
                        printf("班级");
                        break;
                    case 50:j += 3;
                        printf("号码");
                        break;
                    case 65:j += 3;
                        printf("地址");
                        break;
                    case 80:j += 3;
                        printf("修改");
                        break;
                    case 95:j += 3;
                        printf("删除");
                        break;
                    default:
                        printf(" ");
                        break;
                }
            }
            else if (i == 2 && j == 2)//排序操作显示
            {
                if (i == fu->mu[USER].x && j == fu->mu[USER].y)//当用户移动到该点处时
                    printf("●排序");
                else
                    printf("□排序");
                int len = strlen("□排序");
                j += len - 2;
            }
            else if (i == fu->mu[ADD].x && j == fu->mu[ADD].y)//增添信息操作显示
            {
                if (i == fu->mu[USER].x && j == fu->mu[USER].y)
                    printf("●增添信息");
                else
                    printf("□增添信息");
                int len = strlen("□增添信息");
                j += len - 2;
            }
            else if (i == HIGH - 1 && j == WIDTH - strlen("□<-翻页->□") - 3)//翻页操作显示
            {
                if (i == fu->mu[USER].x && j == fu->mu[USER].y)
                    printf("●<-翻页->□");
                else if (i == fu->mu[USER].x && fu->mu[USER].y == fu->mu[RTURN].y)
                    printf("□<-翻页->●");
                else
                    printf("□<-翻页->□");
                j += strlen("□<-翻页->□") - 3;
            }
            else if (i == fu->mu[SEARCH].x && j == fu->mu[SEARCH].y)//搜索操作显示
            {
                char input[20];
                strcpy(input, target);
                if (i == fu->mu[USER].x && j == fu->mu[USER].y)
                    printf("搜索●");
                else if (strlen(input) > 0)
                {
                    printf("搜索:");
                    printf("%s", input);//获取传输数组的信息将其打印在输入栏中
                    j += strlen(input);
                }
                else
                    printf("搜索□");
                j += 4;
            }
            else if (i == fu->mu[QUIT].x && j == fu->mu[QUIT].y)//退出操作显示
            {
                if (i == fu->mu[USER].x && j == fu->mu[USER].y)
                {
                    printf("●退出");
                }
                else
                    printf("□退出");
                j += 4;
            }
            else if (((page == pagesum && i > fu->mu[NUM].x && i < fu->mu[NUM].x + len % 20 + 1) || (page < pagesum && i > fu->mu[NUM].x && i < fu->mu[NUM].x + 21)) && j > 1 && j < WIDTH)//访问并显示链表数据
            {
                if (j == fu->mu[NUM].y)
                {
                    printf("%s", p->stu.num);
                    j += strlen(p->stu.num) - 1;
                }
                else if (j == fu->mu[NAME].y)
                {
                    printf("%s", p->stu.name);
                    j += strlen(p->stu.name) - 1;
                }
                else if (j == fu->mu[CLASS].y)
                {
                    printf("%s", p->stu.class);
                    j += strlen(p->stu.class) - 1;
                }
                else if (j == fu->mu[PHONE].y)
                {
                    printf("%s", p->stu.phone);
                    j += strlen(p->stu.phone) - 1;
                }
                else if (j == fu->mu[ADDR].y)
                {
                    printf("%s", p->stu.addr);
                    j += strlen(p->stu.addr) - 1;
                }
                else if (j == fu->mu[MODIFY].y)
                {
                    if (fu->mu[USER].x == fu->mu[MODIFY].x + p->get_number(*p) - (page - 1) * 20 && fu->mu[USER].y == fu->mu[MODIFY].y)
                        printf("●");
                    else
                        printf("□");
                }
                else if (j == fu->mu[DEL].y)
                {
                    if (fu->mu[USER].x == fu->mu[DEL].x + p->get_number(*p) - (page - 1) * 20 && fu->mu[USER].y == fu->mu[DEL].y)
                        printf("●");
                    else
                        printf("□");
                }
                else
                    printf(" ");
            }
            else
                printf(" ");
        }
        printf("\n");
    }
}

void Operate(struct Menu* fu, LinkList head)//主操作函数
{
    fu->get_add(fu, *head);
    fu->get_user(fu, *head);
    int n = Start();
    gotoxy(0, 0);
    while (n)
    {
        ShowMenu(fu, *head, "\0");
        n = fu->Input(fu, head);
        gotoxy(0, 0);
    }
}

void get_add(struct Menu* fu, struct ListNode head)//获取增添信息选项的坐标
{
    int len = head.Length(&head);
    int pagesum = len / 20 + 1;
    if (page != pagesum)//当页数不为最大页数时不显示增添选项
        fu->mu[ADD].x = fu->mu[ADD].y = -1;
    else
    {
        fu->mu[ADD].x = fu->mu[NUM].x + len % 20 + 1;
        fu->mu[ADD].y = 48;
    }
}

void get_user(struct Menu* fu, struct ListNode head)//初始化移动操作坐标
{
    int len = head.Length(&head);
    int pagesum = len / 20 + 1;
    int count = len % 20;
    if (page == pagesum && count == 0) {
        fu->mu[USER].x = fu->mu[ADD].x;
        fu->mu[USER].y = fu->mu[ADD].y;
    }
    else
    {
        fu->mu[USER].x = fu->mu[MODIFY].x + 1;
        fu->mu[USER].y = fu->mu[MODIFY].y;
    }
}

int Input(struct Menu* fu, struct ListNode* head)//接受按键字符进行相应操作
{
    char input = _getch();
    int length = head->Length(head);
    int pagesum = length / 20 + 1;
    int count = length % 20;
    switch (input)
    {
        case 'w'://上移
            if ((page == pagesum && fu->mu[USER].x > fu->mu[NUM].x + 1 && fu->mu[USER].x <= fu->mu[NUM].x + count) || (page < pagesum && fu->mu[USER].x > fu->mu[NUM].x + 1 && fu->mu[USER].x <= fu->mu[NUM].x + 20))//链表访问范围内
                fu->mu[USER].x--;
            else if (fu->mu[USER].x == fu->mu[ADD].x && fu->mu[USER].y == fu->mu[ADD].y)//位于增添信息处时
            {
                if (page == pagesum && count == 0)//该页面为最大页面并且改页没有其他数据时
                {
                    fu->mu[USER].x = fu->mu[SEARCH].x;
                    fu->mu[USER].y = fu->mu[SEARCH].y;
                }
                else
                {
                    fu->mu[USER].x--;
                    fu->mu[USER].y = fu->mu[MODIFY].y;
                }
            }
            else if (fu->mu[USER].x == fu->mu[LTURN].x)//当位于翻页与退出处时
            {
                if (page == pagesum)
                {
                    fu->mu[USER].x = fu->mu[ADD].x;
                    fu->mu[USER].y = fu->mu[ADD].y;
                }
                else
                {
                    fu->mu[USER].x = fu->mu[MODIFY].x + 20;
                    fu->mu[USER].y = fu->mu[MODIFY].y;
                }
            }
            else if (fu->mu[USER].x == fu->mu[NUM].x + 1)//位于第一个信息处时
            {
                fu->mu[USER].x = fu->mu[SEARCH].x;
                fu->mu[USER].y = fu->mu[SEARCH].y;
            }
            break;
        case 's'://下移
            if ((page == pagesum && fu->mu[USER].x >= fu->mu[NUM].x + 1 && fu->mu[USER].x < fu->mu[NUM].x + count) || (page < pagesum && fu->mu[USER].x >= fu->mu[NUM].x + 1 && fu->mu[USER].x < fu->mu[NUM].x + 20))//链表大小范围内
                fu->mu[USER].x++;
            else if ((page == pagesum && fu->mu[USER].x == fu->mu[ADD].x && fu->mu[USER].y == fu->mu[ADD].y) || (page < pagesum && fu->mu[USER].x == fu->mu[MODIFY].x + 20))//位于增添信息处时
            {
                fu->mu[USER].x = fu->mu[LTURN].x;
                fu->mu[USER].y = fu->mu[LTURN].y;
            }
            else if (fu->mu[USER].x == fu->mu[ADD].x - 1)//位于最后一个信息时
            {
                fu->mu[USER].x = fu->mu[ADD].x;
                fu->mu[USER].y = fu->mu[ADD].y;
            }
            else if (fu->mu[USER].x == fu->mu[SORT].x)//位于排序处时
            {
                if (page == pagesum && count == 0)
                {
                    fu->mu[USER].x = fu->mu[ADD].x;
                    fu->mu[USER].y = fu->mu[ADD].y;
                }
                else
                {
                    fu->mu[USER].x = fu->mu[MODIFY].x + 1;
                    fu->mu[USER].y = fu->mu[MODIFY].y;
                }
            }
            break;
        case 'a'://左移
            if (((page < pagesum && fu->mu[USER].x > fu->mu[DEL].x && fu->mu[USER].x <= fu->mu[DEL].x + 20) || (page == pagesum && fu->mu[USER].x > fu->mu[DEL].x && fu->mu[USER].x < fu->mu[DEL].x + fu->mu[ADD].x)) && fu->mu[USER].y == fu->mu[DEL].y)
                fu->mu[USER].y = fu->mu[MODIFY].y;
            else if (fu->mu[USER].x == fu->mu[RTURN].x && fu->mu[USER].y == fu->mu[RTURN].y)
            {
                fu->mu[USER].x = fu->mu[LTURN].x;
                fu->mu[USER].y = fu->mu[LTURN].y;
            }
            else if (fu->mu[USER].x == fu->mu[SEARCH].x && fu->mu[USER].y == fu->mu[SEARCH].y)
            {
                fu->mu[USER].x = fu->mu[SORT].x;
                fu->mu[USER].y = fu->mu[SORT].y;
            }
            else if (fu->mu[USER].x == fu->mu[LTURN].x && fu->mu[USER].y == fu->mu[LTURN].y)
                fu->mu[USER].y = fu->mu[QUIT].y;
            break;
        case 'd'://右移
            if (((page < pagesum && fu->mu[USER].x > fu->mu[MODIFY].x && fu->mu[USER].x <= fu->mu[MODIFY].x + 20) || (page == pagesum && fu->mu[USER].x > fu->mu[MODIFY].x && fu->mu[USER].x < fu->mu[MODIFY].x + fu->mu[ADD].x)) && fu->mu[USER].y == fu->mu[MODIFY].y)
                fu->mu[USER].y = fu->mu[DEL].y;
            else if (fu->mu[USER].x == fu->mu[LTURN].x && fu->mu[USER].y == fu->mu[LTURN].y)
            {
                fu->mu[USER].x = fu->mu[RTURN].x;
                fu->mu[USER].y = fu->mu[RTURN].y;
            }
            else if (fu->mu[USER].x == fu->mu[SORT].x && fu->mu[USER].y == fu->mu[SORT].y)
            {
                fu->mu[USER].x = fu->mu[SEARCH].x;
                fu->mu[USER].y = fu->mu[SEARCH].y;
            }
            else if (fu->mu[USER].x == fu->mu[QUIT].x && fu->mu[USER].y == fu->mu[QUIT].y)
                fu->mu[USER].y = fu->mu[LTURN].y;
            break;
        case '\r'://Enter键确认
            if (fu->mu[USER].x == fu->mu[ADD].x && fu->mu[USER].y == fu->mu[ADD].y)//点击增添信息
            {
                system("cls");
                fu->Add(fu, head);
            }
            else if (fu->mu[USER].x == fu->mu[LTURN].x && fu->mu[USER].y == fu->mu[LTURN].y)//点击左翻页
                fu->Lturn(fu, *head);
            else if (fu->mu[USER].x == fu->mu[RTURN].x && fu->mu[USER].y == fu->mu[RTURN].y)//点击右翻页
                fu->Rturn(fu, *head);
            else if (fu->mu[USER].y == fu->mu[DEL].y)//点击删除
            {
                int number = (page - 1) * 20 + fu->mu[USER].x - fu->mu[DEL].x;//获取信息在链表中对应的number值
                fu->MenuDelete(fu, head, number);
            }
            else if (fu->mu[USER].x == fu->mu[SORT].x && fu->mu[USER].y == fu->mu[SORT].y)//点击排序
            {
                struct ListNode* low = head->next;
                struct ListNode* high = head->Search(head, length);
                head->Sort(head, low, high);
                head->savefile(head);//在文件中存储新增数据
            }
            else if (fu->mu[USER].y == fu->mu[MODIFY].y)//点击修改键
            {
                int number = (page - 1) * 20 + fu->mu[USER].x - fu->mu[MODIFY].x;//先获取该信息在链表中的number值
                fu->Modify(fu, head, number);
            }
            else if (fu->mu[USER].x == fu->mu[SEARCH].x && fu->mu[USER].y == fu->mu[SEARCH].y)//点击搜索键
            {
                fu->MenuSearch(fu, *head);
            }
            else if (fu->mu[USER].x == fu->mu[QUIT].x && fu->mu[USER].y == fu->mu[QUIT].y)//点击退出键
            {
                return fu->Quit(fu, head);
            }
            break;
    }
    return 1;
}

void ShowAdd(LinkList head, const char* target, int x)//显示添加信息与修改信息时的界面
{
    int i;
    int j;
    static char num[15];//使用静态数组
    static char name[20];
    static char class[30];
    static char phone[15];
    static char addr[30];

    if (x == 16)//通过传输的x进行判断是否清空静态数组中的信息
    {
        memset(num, 0, sizeof(char) * 15);
        memset(name, 0, sizeof(char) * 20);
        memset(class, 0, sizeof(char) * 30);
        memset(phone, 0, sizeof(char) * 15);
        memset(addr, 0, sizeof(char) * 30);
        return;
    }

    for ( i = 1; i <= HIGH; i++)
    {
        for ( j = 1; j <= WIDTH; j++)
        {
            if (i == 1 || i == HIGH)
            {
                if (i == 1 && j == 1)printf("╔");
                else if (i == 1 && j == WIDTH)printf("╗");
                else if (i == HIGH && j == 1)printf("╚");
                else if (i == HIGH && j == WIDTH)printf("╝");
                else
                    printf("═");
            }
            else if (j == 1 || j == WIDTH)
                printf("║");
            else if (j == 44)//显示输入框中信息
            {
                switch (i)
                {
                    case 11:
                        printf("学号：");
                        j += 5;
                        if (x == 11)
                        {
                            strncpy(num, target, 9);
                        }
                        if (strlen(num) != 0)
                        {
                            printf("%s", num);
                            j += strlen(num);
                        }
                        if (x == 10)//通过传输的x判断学号是否存在
                        {
                            printf("该学号已存在");
                            j += 12;
                        }
                        break;
                    case 12:
                        printf("姓名：");
                        j += 5;
                        if (x == 12)
                        {
                            strcpy(name, target);
                        }
                        if (strlen(name) != 0)
                        {
                            printf("%s", name);
                            j += strlen(name);
                        }
                        break;
                    case 13:
                        printf("班级：");
                        j += 5;
                        if (x == 13)
                        {
                            strncpy(class, target, 28);
                        }
                        if (strlen(class) != 0)
                        {
                            printf("%s", class);
                            j += strlen(class);
                        }
                        break;
                    case 14:
                        printf("号码：");
                        j += 5;
                        if (x == 14)
                        {
                            strcpy(phone, target);
                        }
                        if (strlen(phone) != 0)
                        {
                            printf("%s", phone);
                            j += strlen(phone);
                        }
                        break;
                    case 15:
                        printf("地址：");
                        j += 5;
                        if (x == 15)
                        {
                            strncpy(addr, target, 28);
                        }
                        if (strlen(addr) != 0)
                        {
                            printf("%s", addr);
                            j += strlen(addr);
                        }
                        break;
                    default:
                        printf(" ");
                        break;
                }
            }
            else
                printf(" ");
        }
        printf("\n");
    }
}

bool input_string(struct Menu* fu, LinkList head, char* target, int x, MenuData f)//实现实时输入
{
    char input;
    char buffer[30] = { 0 };//缓存数组获取目标数组
    strcpy(buffer, target);
    int bufferindex = strlen(target) - 1;//记录下标位置
    do
    {
        gotoxy(0, 0);
        if (f == ADD)
            ShowAdd(head, buffer, x);
        else
            ShowMenu(fu, *head, buffer);
        input = _getch();
        if (input == 27)//esc键退出
        {
            Sleep(30);
            gotoxy(0, 0);
            if (f == ADD)
            {
                ShowAdd(head, "\0", 16);//退出时将已输入的信息删除
                Sleep(30);
                gotoxy(0, 0);
            }
            return false;
        }
        else if (input == '\b')//点击退格键删除
        {
            if (bufferindex > 0 && buffer[bufferindex - 1] >= ' ' && buffer[bufferindex - 1] <= '~')
            {
                if (x == 10)
                    x = 11;
                buffer[--bufferindex] = buffer[bufferindex + 1];
                buffer[bufferindex + 1] = 0;
            }
            else if (bufferindex > 0)//删除的为汉字时由于汉字占两个字节，因此需要改变两个数组单元的信息
            {
                bufferindex -= 2;
                buffer[bufferindex] = buffer[bufferindex + 2];
                buffer[bufferindex + 1] = 0;
                buffer[bufferindex + 2] = 0;
            }
        }
        else
        {
            if ((x == 11 || x == 14) && input != '\r' && !(input >= '0' && input <= '9'))//输入学号和号码时必须使用数字
                continue;
            else if (input == '\r')
            {
                buffer[bufferindex] = '\0';
                if (bufferindex < 4)//输入大小必须大于等于4字符
                {
                    input = 1;
                    buffer[bufferindex] = '|';
                }
                else if ((x == 11 || x == 10) && head->is_exist(head, buffer))//检查学号是否重复
                {
                    input = 1;
                    x = 10;
                    buffer[bufferindex] = '|';
                }
                else
                    break;
            }
            else if ((x == 11 || x == 10) && bufferindex >= 8)//限制输入各类型信息时的大小
                continue;
            else if (x == 12 && bufferindex >= 18)
                continue;
            else if (x == 13 && bufferindex >= 28)
                continue;
            else if (x == 14 && bufferindex >= 11)
                continue;
            else if (x == 15 && bufferindex >= 28)
                continue;
            else//正常输入
            {
                if (x == 10)
                    x = 11;
                buffer[++bufferindex] = buffer[bufferindex - 1];
                buffer[bufferindex - 1] = input;
            }
        }
    } while (input != '\r');//以按回车键为判断条件
    buffer[bufferindex] = '\0';
    Sleep(100);
    gotoxy(0, 0);
    if (f == ADD)
        ShowAdd(head, buffer, x);
    else
    {
        ShowMenu(fu, *head, buffer);
        gotoxy(0, 0);
    }
    strncpy(target, buffer, bufferindex);//将最终的缓存数组赋值给目标数组
    return true;
}

void Add(struct Menu* fu, LinkList head)//实现增添操作
{
    int pagesum = head->Length(head) / 20 + 1;
    char num[15] = "|";
    char name[20] = "|";
    char class[30] = "|";
    char phone[15] = "|";
    char addr[30] = "|";//初始化为输入光标

    if (!input_string(fu, head, num, 11, ADD))//放弃添加则结束函数
        return;
    if (!input_string(fu, head, name, 12, ADD))
        return;
    if (!input_string(fu, head, class, 13, ADD))
        return;
    if (!input_string(fu, head, phone, 14, ADD))
        return;
    if (!input_string(fu, head, addr, 15, ADD))
        return;
    ShowAdd(head, "\0", 16);//清除函数中的静态数组残存的输入信息
    gotoxy(0, 0);
    head->Insert(head, num, name, class, phone, addr, 0);
    head->savefile(head);//在文件中存储新增数据
    pagesum = head->Length(head) / 20 + 1;
    if (page < pagesum)//当添加数据改页数据达到上限
        fu->Rturn(fu, *head);
    fu->get_add(fu, *head);//每次添加完数据更新添加选项的坐标
    fu->mu[USER].x = fu->mu[ADD].x;
    fu->mu[USER].y = fu->mu[ADD].y;
}

void MenuDelete(struct Menu* fu, LinkList head, int number)//实现删除操作
{
    struct ListNode* p = head->Search(head, number);
    int len = head->Length(head);
    if (p == NULL)
        return;
    else
        Delete(head, p->get_number(*p));
    fu->get_add(fu, *head);//每次删除完数据更新添加选项的坐标
    if (page == len / 20 + 1 && len % 20 == 1)//只剩一条数据时
    {
        fu->mu[USER].x = fu->mu[ADD].x;
        fu->mu[USER].y = fu->mu[ADD].y;
    }
    else if (number == len)//删除最后一个数据
        fu->mu[USER].x--;
    head->savefile(head);//在文件中存储新增数据
}

void Modify(struct Menu* fu, LinkList head, int number)//实现修改功能
{
    struct ListNode* p = head->Search(head, number);
    char num[15] = { 0 };
    char name[20] = { 0 };
    char class[30] = { 0 };
    char phone[15] = { 0 };
    char addr[30] = { 0 };//用于传入输入函数
    char num2[15] = { 0 };
    char name2[20] = { 0 };
    char class2[30] = { 0 };
    char phone2[15] = { 0 };
    char addr2[30] = { 0 };//用于备份

    strcpy(num, p->stu.num);
    num[strlen(p->stu.num)] = '|';
    strcpy(name, p->stu.name);
    name[strlen(p->stu.name)] = '|';
    strcpy(class, p->stu.class);
    class[strlen(p->stu.class)] = '|';
    strcpy(phone, p->stu.phone);
    phone[strlen(p->stu.phone)] = '|';
    strcpy(addr, p->stu.addr);
    addr[strlen(p->stu.addr)] = '|';

    strcpy(num2, p->stu.num);
    strcpy(name2, p->stu.name);
    strcpy(class2, p->stu.class);
    strcpy(phone2, p->stu.phone);
    strcpy(addr2, p->stu.addr);

    head->Delete(head, number);//先删除原有结点，防止干扰查重
    if (!input_string(fu, head, num, 11, ADD))//放弃修改则直接将备份插入链表原有位置
    {
        head->Insert(head, num2, name2, class2, phone2, addr2, number);
        return;
    }
    if (!input_string(fu, head, name, 12, ADD))
    {
        head->Insert(head, num2, name2, class2, phone2, addr2, number);
        return;
    }
    if (!input_string(fu, head, class, 13, ADD))
    {
        head->Insert(head, num2, name2, class2, phone2, addr2, number);
        return;
    }
    if (!input_string(fu, head, phone, 14, ADD))
    {
        head->Insert(head, num2, name2, class2, phone2, addr2, number);
        return;
    }
    if (!input_string(fu, head, addr, 15, ADD))
    {
        head->Insert(head, num2, name2, class2, phone2, addr2, number);
        return;
    }
    num[strlen(num) - 1] = '\0';
    name[strlen(name) - 1] = '\0';
    class[strlen(class) - 1] = '\0';
    phone[strlen(phone) - 1] = '\0';
    addr[strlen(addr) - 1] = '\0';
    ShowAdd(head, "\0", 16);
    Sleep(100);
    gotoxy(0, 0);
    head->Insert(head, num, name, class, phone, addr, number);//修改完成后再插入原有位置
    head->savefile(head);//在文件中存储新增数据
}

void Lturn(struct Menu* fu, struct ListNode head)//实现左翻页
{
    if (page > 1)
    {
        page--;
        if (page + 1 == head.Length(&head) / 20 + 1)//翻页前是最后一页时
            fu->get_add(fu, head);
    }
    else
        return;
}

void Rturn(struct Menu* fu, struct ListNode head)//实现右翻页
{
    if (page < head.Length(&head) / 20 + 1)
    {
        page++;
        if (page == head.Length(&head) / 20 + 1)//翻页后是最后一页时
            fu->get_add(fu, head);
    }
    else
        return;
}

void MenuSearch(struct Menu* fu, struct ListNode head)//实现搜索功能
{
    fu->mu[USER].x = fu->mu[USER].y = 0;
    char buffer[20] = "|";
    if (!input_string(fu, &head, buffer, 12, SEARCH) || !head.next)//放弃搜素或者链表为空时
    {
        fu->mu[USER].x = fu->mu[SEARCH].x;
        fu->mu[USER].y = fu->mu[SEARCH].y;
        return;
    }
    int number = head.search_number(&head, buffer);//获取查询信息的number值
    if (number == 0)//若不存在则搜索失败
    {
        fu->mu[USER].x = fu->mu[SEARCH].x;
        fu->mu[USER].y = fu->mu[SEARCH].y;
        return;
    }
    else//若存在则改变当前页数
    {
        int pagesum = head.Length(&head) / 20 + 1;
        page = (number - 1) / 20 + 1;
        if (number % 20 == 0)
            fu->mu[USER].x = fu->mu[MODIFY].x + 20;
        else
            fu->mu[USER].x = fu->mu[MODIFY].x + number % 20;
        fu->mu[USER].y = fu->mu[MODIFY].y;
        fu->get_add(fu, head);
    }
}

int Quit(struct Menu* fu,LinkList head)
{
    int len = head->Length(head);
    for (int i = 0; i < len; i++)
    {
        head->Delete(head, len - i);
    }
    return 0;
}
