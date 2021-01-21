int g;
extern int sum(int, int, int, int, int, int);
int main()
{
    int a, b, c, d, e, f;
    a = b = c = d = e = f = 1;
    g = sum(a, b, c, d, e, f);
    while (1)
        ;
    return 0;
}