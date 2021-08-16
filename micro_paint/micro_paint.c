#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    float width;
    float height;
    char c;

} t_draw;

static void _free_(void *zone)
{
    if (zone)
        free(zone);
}
static int  _exit_(char *msg, int ret)
{
    printf ("%s\n", msg);
        return (ret);
}


static int  check_zone(FILE *file, t_zone *paper, char **zone)
{
    int i = 0;

   if ((i = fscanf(file, "%d %d %c\n", &paper->width, &paper->height, &paper->c)) != 3) //取得矩形长宽值
        return (0);
    if (paper->width > 300 || paper->width < 0 ||  paper->height > 300 || paper->height < 0)
        return (0);
    if (i == -1)
        return (0);
    *zone = calloc(paper->width * paper->height, sizeof(char));
    if (!*zone)
        return (0);
    memset (*zone, paper->c, (paper->width * paper->height) * sizeof(char)); // 把char c 放进zone里面
    return (1);
}

static int check_rect(float i, float j, t_draw *draw)
{
    
    if (i < draw->x || j < draw->y || i > draw->x + draw->width || j > draw->y + draw->height)// 小于起点大于图像大小的点 不画
        return (0);
    // if (i - draw->x < 1. || j - draw->y < 1. || draw->x + draw->width - i < 1. || draw->y + draw->height -j < 1.) 
    //     return (2); //i 表示每趟的x值都画 ， j表示没个第y趟都画 ，i的值是最大时候都画 , j的值最大时候都画
    if(i < draw->x + 1 || i > draw->x + draw->width - 1  || j < draw->y + 1 || j > draw->y + draw->height - 1)
            return (2);
    return (1);  //画c
        
}
static void draw_rect(t_zone *paper, char **zone, t_draw *draw)// 画一个图
{
    int i;
    int j;

    if (draw->type == 'R')
    {
        j = 0;
        while (j < paper->height)
        {
            i = 0;
            while (i < paper->width)
            {
                if (check_rect((float)i, (float)j, draw))
                    (*zone)[j * paper->width + i] = draw->c;     
                i++;
            }
            j++;
        }
        
    }
    else if (draw->type == 'r')
    {
        j = 0;//全画
        while (j < paper->height)
        {
            i = 0;
            while (i < paper->width)
            {
                if (check_rect((float)i, (float)j, draw) == 2)  //只画框
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
   // int i = 0;
    while ((j = fscanf(file, "%c %f %f %f %f %c\n", &draw.type, &draw.x, &draw.y, &draw.width, &draw.height, &draw.c)) == 6)
    {
       if (!(draw.width > 0. && draw.height > 0.) && !(draw.type == 'r' && draw.type == 'R'))
            return (0);
        draw_rect(paper, zone, &draw);
    }
    if (j != -1)
        return (0);
    return (1);  
}

static void print_all(char *zone, t_zone paper)  //
{
    int i;
    int j;

    j = 0;
    while (j < paper.height)
    {
        i = 0;
        while (i < paper.width)
        {
            printf("%c", zone[j * paper.width + i++]);
        }
        printf("\n");
        
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
    if (!(check_draw_rect(file, &paper, &zone)))
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
