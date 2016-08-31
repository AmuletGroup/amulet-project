float computeVar(float val_array[],int len,float average){
    float rv=0;
    i = 0;
    /*if (val_array[i] > val_array[i+1]) {
    	printf("Hey");
    }
    rv+=val_array[i];
    rv+=((val_array[i]-average)*(val_array[i]-average)/(float)val_array[i]);*/
    val_array[i]+=((val_array[i]-average)*(val_array[i]-average)/(float)len);
    val_array[i]+=(val_array[i]-average);
    return rv;
}

int bar(int x)
{
	CoreLedOn(1);
	CoreLedOff();
	if (x[0] == 0) return 1;
	//if (x > 0) return foo(x-1);
	//if (x < 0) return foo(x+1);
	if (x[0] < 0) return 2;
}

int foo(int x[])
{
	if (x[0] == 0) return x[0];
	//if (x > 0) return foo(x-1);
	//if (x < 0) return foo(x+1);
	if (x[0] < 0) return 2;
}

int main(void) {
	int x;
	int y, z;
	int k[22];
	int a[100],b;
	int m,l[20];
	int c[120],d[240];
	//for(; scanf("%d",&x) > 0 && x != 42; printf("%d\n", x));
	//goto start;
	int i = foo(a);
	c[1] = 120;
	l[3] *= 288;
	//Cases to be considered for array access and array assignment
	l[3] = c[1];
	x = c[1];
	int less = c[1];
	if (l[3] == c[1]) {
		l[3] = 121;
	} else if (l[3] < c[1]) {
		l[3] = 119;
	}
	return l[3];
	l[1] = 2;
	c[2] = 4;
	y = 123;
	y = (y+1)%4134;
	i = 0;
	l[i++] = 2;
	//To be handled?
	//x = c[1*l[1]];
	//l[3] = c[2] = x;
	CoreLedOn(1);
	CoreLedOn();
	return i;
}

