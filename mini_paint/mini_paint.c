#include <stdio.h>
#include <string.h> // memset
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
typedef struct s_zone
{
    int width;
    int height;
    char c;
}  t_zone;

typedef struct s_draw
{
    char type;
    float x;
    float y;
    float radius;
    char c;

} t_draw;

static void ft_putchar(char c)
{
    write(1, &c, 1);
}

static void ft_putstr(char *str)
{
    while (*str)
        write(1, str++, 1);
}
static void _free_(void *zone)
{
    if (zone)
        free(zone);
}
static int  _exit_(char *msg, int ret)
{
    if (msg)
        ft_putstr(msg);
    return (ret);
}


static int  check_zone(FILE *file, t_zone *paper, char **zone)
{
    int i = 0;

   if ((i = fscanf(file, "%d %d %c\n", &paper->width, &paper->height, &paper->c)) != 3) //取得矩形长宽值
        return (0);
    if (paper->width > 300 || paper->width <= 0 ||  paper->height > 300 || paper->height <= 0)
        return (0);
    if (i == -1)
        return (0);
    *zone = calloc(paper->width * paper->height, sizeof(char));
    if (!*zone)
        return (0);
    memset (*zone, paper->c, paper->width * paper->height); // 把char c 放进zone里面
    return (1);
}

static int creat_c(float i, float j, t_draw *draw)
{
    float p; //亮点直接的距离

    p = sqrtf(powf(i - draw->x, 2.) + powf(j - draw->y, 2));//sqrtf返回arg的平方根 负数没有平方根  //powf 是 i - x的2次方
    //p = sqrtf((i - draw->x) * (i - draw->x) + (j - draw->y) * (j - draw->y));
    if (p <= draw->radius)
    {
        if (draw->radius - p < 1.)  //直径-去两点直接距离 小于1
            return (2);
        return (1);
    }  
    return (0);

        
}
static void draw_c(t_zone *paper, char **zone, t_draw *draw)// 画一个图
{
    int i;
    int j;

    if (draw->type == 'C')
    {
        j = 0;
        while (j < paper->height)
        {
            i = 0;
            while (i < paper->width)
            {
                if (creat_c((float)i, (float)j, draw))
                    (*zone)[j * paper->width + i] = draw->c;     
                i++;
            }
            j++;
        }
        
    }
    else if (draw->type == 'c')
    {
        j = 0;//全画
        while (j < paper->height)
        {
            i = 0;
            while (i < paper->width)
            {
                if (creat_c((float)i, (float)j, draw) == 2)  //只画框
                    (*zone)[j * paper->width + i] = draw->c;
                i++;
            }
            j++;
        }
        
    }

}
static int check_all(FILE *file, t_zone *paper, char **zone)   //画好几个图
{
    t_draw draw;
    int j = 0;
    
    while ((j = fscanf(file, "%c %f %f %f %c\n", &draw.type, &draw.x, &draw.y, &draw.radius, &draw.c)) == 5)
    {
       if (!(draw.radius > 0.) || (draw.type != 'c' && draw.type != 'C'))
            return (0);
        draw_c(paper, zone, &draw);
    }
    if (j != -1)
        return (0);
    return (1);  
}

static void print_all(char *zone, t_zone paper) 
{
    int i;
    int j;

    j = 0;
    while (j < paper.height)
    {
        i = 0;
        while (i < paper.width)
            ft_putchar(zone[j * paper.width + i++]);
        ft_putchar('\n');
        
        j++;
    }
}
int main(int ac, char **av)
{
    t_zone paper;
    FILE *file;
    char *zone;

    memset(&paper, 0, sizeof(t_zone));
    if (ac != 2)
        return (_exit_("1Error: argument\n", 1));
    if (!(file = fopen(av[1], "r")))                    
        return (_exit_("4Error: Operation file corrupted\n", 1));
    if (!(check_zone(file, &paper, &zone)))
    {
        _free_(zone);
        fclose(file);
        return (_exit_("2Error: Operation file corrupted\n", 1));
    }
    if (!(check_all(file, &paper, &zone)))
    {
        _free_(zone);
        fclose(file);
        return (_exit_("3Error: Operation file corrupted\n", 1));
    }
    print_all(zone, paper);
     _free_(zone);
    fclose(file);
    return (0);
}