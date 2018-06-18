
#include<stdio.h>
#include<graphics.h>
#include<dos.h>
#include<conio.h>
#include<math.h>
#include<string.h>

int op[20],xc1[20],yc1[20],xc2[20],yc2[20],count=0,lcount=0,fcount=0;
FILE *fp;

union REGS in, out;
//float mouseb,mousetx,mousety,scalefactor;

void matrixtrans(int x,int y,int tx,int ty,int *t)
{
     t[0]=x+tx;
     t[1]=y+ty;
}
void matrixscale(int x1,int y1,int x2,int y2,float s,int *t)
{
	matrixtrans(x2,y2,-x1,-y1,t);

	t[0]=t[0]*s;
	t[1]=t[1]*s;

	matrixtrans(t[0],t[1],x1,y1,t);

}

void matrixrot(int x1,int y1,int x2,int y2,int d,int *t)
{
int u,v;
//float leno,lenn,sl;
//	leno=sqrt(((y2-y1)*(y2-y1))+((x2-x1)*(x2-x1)));
	matrixtrans(x2,y2,-x1,-y1,t);
	u=t[0];
	v=t[1];
	t[0]=(u*cos(d*3.14/180))-(v*sin(d*3.14/180));
	t[1]=(u*sin(d*3.14/180))+(v*cos(d*3.14/180));

	matrixtrans(t[0],t[1],x1,y1,t);
//	lenn=sqrt(((t[1]-y1)*(t[1]-y1))+((t[0]-x1)*(t[0]-x1)));

//	sl=leno/lenn;
//	matrixscale(t[0],t[1],x1,y1,sl,t);
}

void dda(float x1,float y1,float x2,float y2,int c)
{

 float delx,dely,adelx,adely;
 float len;
 float X,Y;
 int i;

 delx=x2-x1;
 dely=y2-y1;

 adelx=abs(delx);
 adely=abs(dely);

 if(adelx>=adely)
  len=adelx;
 else
  len=adely;


 delx=delx/len;
 dely=dely/len;
 if(delx==0)
 X= x1;
 else
 X= x1 + 0.5 * (abs(delx)/delx);  // sign
 if(dely==0)
 Y = y1;
 else
 Y= y1 + 0.5 * (abs(dely)/dely);
 i=0;

 while(i<=len)
 {
  putpixel((int)X,(int)Y,c);
  X+=delx;
  Y+=dely;
  i++;

 }
}
void triangle(float x1,float y1,float x2,float y2,int c)
{
dda(x1,y2,(x1+x2)/2,y1,c);
dda((x1+x2)/2,y1,x2,y2,c);
dda(x1,y2,x2,y2,c);
}

void rect(float x1,float y1,float w,float h,int c)
{
dda(x1,y1,x1+w,y1,c);
dda(x1,y1,x1,y1-h,c);
dda(x1+w,y1,x1+w,y1-h,c);
dda(x1,y1-h,x1+w,y1-h,c);
}



int getselectedshape(float x,float y)
{
int s;
if(x>=20 && x<=50)
{
	if(y>=100 && y<=130)
	s=1; //line selected
	else if(y>=150 && y<=180)
	s=2; //rectangle selected
	else if(y>=200 && y<=230)
	s=3; //triangle
	else if(y>=250 && y<=280)
	s=4; //circle
	
}

return s;
}


int selecttrue(int layerflag,int x,int y)
{
 if(op[layerflag]==1)
 {
  int x1=xc1[layerflag],y1=yc1[layerflag],x2=xc2[layerflag],y2=yc2[layerflag];
  int r=y2-y1;
  int s=x1-x2;
  int t=x2*y1-y2*x1;
  float d=((r*x+s*y+t)/abs(sqrt(r*r+s*s)));
  if(abs(d)<=5 && x>=x1 && x<=x2 && y>=y1 && y<=y2)
  {
   return 1;
  }
  else
  {
   return 0;
  }
 }

 else if(op[layerflag]==2)
 {
	  int x1=xc1[layerflag],y1=yc1[layerflag],x2=xc2[layerflag],y2=yc1[layerflag];
	  int r=y2-y1;
	  int s=x1-x2;
	  int t=x2*y1-y2*x1;
	 // float d=((r*x+s*y+t)/abs(sqrt(r*r+s*s)));
	  if(x>=xc1[layerflag] && x<=xc2[layerflag] && y>=yc1[layerflag] && y<=yc2[layerflag])
	  {
	   return 1;
	  }
	  else
	  {
	   return 0;
	  }
 }
 else if(op[layerflag]==3)
 {
	if(x>=xc1[layerflag] && x<=xc2[layerflag] && y>=yc1[layerflag] && y<=yc2[layerflag])
	  {
	   return 1;
	  }
	  else
	  {
	   return 0;
	  }
	  
	  }
 
 
 else if(op[layerflag]==4)
 {
	float radius=abs(sqrt(((yc2[layerflag]-yc1[layerflag])*(yc2[layerflag]-yc1[layerflag]))+((xc2[layerflag]-xc1[layerflag])*(xc2[layerflag]-xc1[layerflag]))));
	if((x*x)+(y*y)<=(radius*radius))
	{ 	return 1; }
	else
		return 0;
 }
}

void createLayer(int cnt,int sr)
{
		rect(70+40*(cnt),479,40,80,2);
		settextstyle(0,1,1);
		setcolor(5);
		if(sr==1)
		outtextxy(70+40*(cnt)+15,430,"Line");
		else if(sr==2)
		outtextxy(70+40*(cnt)+15,430,"Rect");
		else if(sr==3)
		outtextxy(70+40*(cnt)+15,430,"Triangle");
		else if(sr==4)
		outtextxy(70+40*(cnt)+15,430,"Circle");		
}

int selectLayer(int x)
{
x=x-70;
	if(x<=70+40*count)
	{
		x=x/40;
		return x;
	}
	else
		return -1;
}
void saving()
{
 FILE *fp;
 int i;

     fp=fopen("out2.txt","wb");
 if(fp==NULL)
 {
  outtextxy(getmaxx()-200,getmaxy()-25,"error in saving");
 }
 /*   for(i=0;i<count;i++)
    {
     st.opar[i]=op;
     st.xc1ar[i]=xc1;
     st.yc1ar[i]=yc1;
     st.xc2ar[i]=xc2;
     st.yc2ar[i]=yc2;
    }
     st.cnt=count;
    fwrite(&st,sizeof(st),1,fp);*/
 // putw(count,fp);

  for(i=0;i<count+1;i++)
  {
  putw(op[i],fp);
//  putc(' ',fp);
  putw(xc1[i],fp);
//  putc(' ',fp);
  putw(yc1[i],fp);
//  putc(' ',fp);+
  putw(xc2[i],fp);
//  putc(' ',fp);
  putw(yc2[i],fp);
//  putc('\n',fp);
  }

 fclose(fp);
}
void load()
{
 FILE *fp;
 int no,i;
  struct arr
   {
    int opar[20];
    int xc1ar[20];
    int yc1ar[20];
    int xc2ar[20];
    int yc2ar[20];
    int cnt;
    }st;

 fp=fopen("out2.txt","rb");


// fread(&st,sizeof(st),1,fp);
/*  count=st.cnt;
  for(i=0;i<count;i++)
  {
   op[i]=st.opar[i];
   xc1[i]=st.xc1ar[i];
   yc1[i]=st.yc1ar[i];
   xc2[i]=st.xc2ar[i];
   yc2[i]=st.yc2ar[i];
 }*/
// fclose(fp);
 // count=getw(fp);
 i=0;
 while((no=getw(fp))!=-1)
 {
   op[i]=no;
   xc1[i]=getw(fp);
   yc1[i]=getw(fp);
   xc2[i]=getw(fp);
   yc2[i]=getw(fp);
   i++;
 }
 count=i;

 fclose(fp);
}

void opArray()
{
int i=0;
	while(i<count)
	{

		  if(op[i]==1)
		  {	dda(xc1[i],yc1[i],xc2[i],yc2[i],14);

//		  clr();
			createLayer(i,op[i]);
		  }
		  else if(op[i]==2)
		  {     setcolor(14);
			rectangle(xc1[i],yc1[i],xc2[i],yc2[i]);
  //			clr();
			createLayer(i,op[i]);  }
		  else if(op[i]==3)
		  {
		  triangle(xc1[i],yc1[i],xc2[i],yc2[i],14);
    //		  clr();
		  createLayer(i,op[i]);
		  }

		  else if(op[i]==4)
		  {    setcolor(14);
			circle(xc1[i],yc1[i],abs(sqrt(((yc2[i]-yc1[i])*(yc2[i]-yc1[i]))+((xc2[i]-xc1[i])*(xc2[i]-xc1[i])))));
    //			clr();
			createLayer(i,op[i]);	}


	  i++;
	}
}


void layout()
{

settextstyle(0,0,1);
setbkcolor(7);
setcolor(1);
rect(1,479,637,477,9);
outtextxy(250,10,"Computer Graphics Project");

dda(570,0,570,400,14); // scaling toolbox
dda(0,400,650,400,14); // layer tool box
dda(70,0,70,480,14); // toolbox
setcolor(12);
outtextxy(8,50,"Options");

setfillstyle(1,12);
bar(240,0,500,30);
setcolor(12);
//settextstyle(0,0,2);

rect(20,130,30,30,9);
dda(25,105,45,125,14); // line tool
rect(20,180,30,30,9);
rect(25,175,20,20,9);  // rect tool
rect(20,230,30,30,9);
dda(25,225,45,225,14);  // draw triangle
dda(25,225,35,210,14);
dda(45,225,35,210,14);
rect(20,280,30,30,14);
circle(35,265,10);



outtextxy(590,50,"SCALE");
rect(590,90,40,20,9);
outtextxy(602,75,"-ve");
rect(590,120,40,20,9);
outtextxy(602,105,"+ve");

//setcolor(5);
outtextxy(580,140,"LAYER");
 rect(590,180,30,20,9);
outtextxy(599,165,"DEL");
//rect(590,180,30,50,9);
//settextstyle(0,1,0);
//outtextxy(602,165,"DEL");
/*
rect(590,210,30,20,9);
outtextxy(602,195,"+"); */

/*
outtextxy(590,240,"MOVE");
rect(590,280,30,20,2);
outtextxy(602,265,"L");
rect(590,310,30,20,2);
outtextxy(602,295,"R");
rect(590,340,30,20,2);
outtextxy(602,325,"T");
rect(590,370,30,20,2);
outtextxy(602,355,"D");

*/

rect(590,380,40,25,9);
outtextxy(595,365,"EXIT");
rect(590,330,40,25,9);
outtextxy(595,315,"LOAD");
rect(590,280,40,25,9);
outtextxy(595,265,"SAVE");


//settextstyle(0,1,0);
setcolor(12);
outtextxy(20,450,"Layer");


}

void clr()
{
clrscr();
}

void newfile()
{

in.x.ax =2;
int86(51,&in,&out);
layout();
clrscr();
in.x.ax =1;
int86(51,&in,&out);

//setbkcolor(0);
}


void main()
{
int gdriver=DETECT,gmode,x1,y1,x2,y2,j;
 int layerflag=-1,layertrue=0,d_x,d_y,objno=-13;
 int *t;
 float radius;

 char c[25],d[1];
 int mousetx,mousety,mouseb,flag=0;
 initgraph(&gdriver,&gmode,"..//bgi");

setcolor(4);
in.x.ax =0;
 int86(51,&in,&out);
 in.x.ax =1;
 int86(51,&in,&out);

 in.x.ax=3;

 int86(51,&in,&out);

//layout();
	while(!kbhit())
	{
	if(fcount==0)
	{

	}
	else
	{

	}
	  layout();
//	setbkcolor(0);
	if(count!=0)
	opArray();
//clr();
	mouseb=out.x.bx;
	mousetx=out.x.cx;
	mousety=out.x.dx;

	

	int86(51,&in,&out);

	if(mouseb==1 && flag==0)
	{
	 x1=out.x.cx;
	 y1=out.x.dx;
	 if(y1>=410)
	 {
		flag=0;
		layerflag=selectLayer(x1);
	 }
	 else if(x1<=70 && y1>=30 && y1<=410)

	 {
		layerflag = -1;
		objno=getselectedshape(x1,y1);
	 }
	 else if(x1>=590 && x1<=620 && y1>=70 && y1<=90)
	 {
		matrixscale(xc1[layerflag],yc1[layerflag],xc2[layerflag],yc2[layerflag],0.5,t);
		xc2[layerflag]=t[0];
		yc2[layerflag]=t[1];
	 }
	 else if(x1>=590 && x1<=620 && y1>=100 && y1<=120)
	 {
		matrixscale(xc1[layerflag],yc1[layerflag],xc2[layerflag],yc2[layerflag],2,t);
		xc2[layerflag]=t[0];
		yc2[layerflag]=t[1];
	 }
	 else if(x1>=590 && x1<=620 && y1>=160 && y1<=180)
	 {
/*
		matrixrot(xc1[layerflag],yc1[layerflag],xc2[layerflag],yc2[layerflag],-5,t);
		xc2[layerflag]=t[0];
		yc2[layerflag]=t[1];  */
		/*if(count<20)
		{
		xc1[count+1] = xc1[layerflag];
		xc2[count+1] = xc2[layerflag];
		yc1[count+1] = yc1[layerflag];
		yc2[count+1] = yc2[layerflag];
		op[count+1] = op[layerflag];
		
		createLayer(count,op[layerflag]);
		count++;
		layerflag = -1;
		
		} */
		
	
	 
		/* matrixrot(xc1[layerflag],yc1[layerflag],xc2[layerflag],yc2[layerflag],5,t);
		xc2[layerflag]=t[0];
		yc2[layerflag]=t[1]; */
		if(layerflag != -1)
		{
		while(layerflag<count)
		{
			xc1[layerflag] = xc1[layerflag+1] ;
			xc2[layerflag] = xc2[layerflag+1] ;
			yc1[layerflag] = yc1[layerflag+1] ;
			yc2[layerflag] = yc2[layerflag+1] ;
			op[layerflag] = op[layerflag+1] ;
			layerflag++ ; 
			
		}
		cleardevice();
		clr();
		layerflag = -1;
		
			opArray();
			
		
		count-- ;
		
	 }
	 }
	 else if(x1>=590 && x1<=630 && y1>=255 && y1<=280)
	 {
		saving();
		printf("saving ...");
	 }

	 else if(x1>=590 && x1<=630 && y1>=355 && y1<=380)
	 {
		exit(0);
	 }
	 else if(x1>=590 && x1<=630 && y1>=305 && y1<=330)
	 {
		load();
		printf("loading....");
	 }

	 else
	 {flag=1;}

	 if(layerflag!=-1)
	 {
		//layertrue=selecttrue(layerflag,x1,y1);
		//if(layertrue==1)
		//{
			if(op[layerflag]==1)  //for line
			{
			dda(xc1[layerflag],yc1[layerflag],xc2[layerflag],yc2[layerflag],2 );
			clr();
			}
			else if(op[layerflag]==2)
			{  setcolor(2);
			   rectangle(xc1[layerflag],yc1[layerflag],xc2[layerflag],yc2[layerflag]);
			  clr();
			   }

			   else if(op[layerflag]==3)
	{
	triangle(xc1[layerflag],yc1[layerflag],xc2[layerflag],yc2[layerflag],2);
	clr();
	}
			else if(op[layerflag]==4)
			{  setcolor(2);
			   radius=abs(sqrt(((yc2[layerflag]-yc1[layerflag])*(yc2[layerflag]-yc1[layerflag]))+((xc2[layerflag]-xc1[layerflag])*(xc2[layerflag]-xc1[layerflag]))));
			   circle(xc1[layerflag],yc1[layerflag],radius);
				clr();
			}

			   else
			{ }
			flag=2;
		//}
	 }

	}

	if(mouseb==1 && flag==1)
	{
	  x2=out.x.cx;
	  y2=out.x.dx;
	  //x1=x2;
	  //y1=y2;


	//x2=out.x.cx;
	//y2=out.x.dx;

	  if(objno==1)            //to draw line
	  { dda(x1,y1,x2,y2,2);
	  clr();
	  }
	  else if(objno==2)	//rectangle
	  { setcolor(2);
	    rectangle(x1,y1,x2,y2);
clr();
		}
	  else if(objno==3)
	  {
	  triangle(x1,y1,x2,y2,2);
	  clr();
	  }
	  else if(objno==4)
	  { setcolor(2);
	    radius=abs(sqrt(((y2-y1)*(y2-y1))+((x2-x1)*(x2-x1))));
	    circle(x1,y1,radius);
		clr();
  }
	 // else if(objno==5)
	  //{ floodfill(x1,y1,0);
  //		}
	  else
	  { }


	  }

	/*
	 if(objno==1)            //to draw line
	  { dda(x1,y1,x2,y2,15); }
	  else if(objno==2)	//rectangle
	  { setcolor(15);
	    rectangle(x1,y1,x2,y2);  }
	  else if(objno==3)
	  {
	  triangle(x1,y1,x2,y2,15);
	  }
	  else if(objno==4)
	  { setcolor(15);
	    radius=abs(sqrt(((y2-y1)*(y2-y1))+((x2-x1)*(x2-x1))));
	    circle(x1,y1,radius); }
	 // else if(objno==5)
	  //{ floodfill(x1,y1,0);	}
	  else
	  { }
	  */





	if(mouseb==0 && flag==1)
	{
	 if(objno==1)
	 { 	op[count]=1;
		xc1[count]=x1;
		yc1[count]=y1;
		xc2[count]=x2;
		yc2[count++]=y2; }
	 else if(objno==2)
	 { 	op[count]=2;
		xc1[count]=x1; yc1[count]=y1; xc2[count]=x2; yc2[count++]=y2;
//		op[count]=3;
 //		xc1[count]=x2; yc1[count]=y1; xc2[count]=x2; yc2[count++]=y2;
 //		op[count]=3;
 //		xc1[count]=x2; yc1[count]=y2; xc2[count]=x1; yc2[count++]=y2;
//		op[count]=3;
//		xc1[count]=x1; yc1[count]=y2; xc2[count]=x1; yc2[count++]=y1;
	 }
	 else if(objno==3)
	 {
	 op[count]=3;
		xc1[count]=x1; yc1[count]=y1; xc2[count]=x2; yc2[count++]=y2;


	 }
	 else if(objno==4)
	 {
		op[count]=4;
		xc1[count]=x1; yc1[count]=y1; xc2[count]=x2; yc2[count++]=y2;
	}
	flag=0;
/*
	  if(objno==1)
	  {
	   dda(x1,y1,x2,y2,0); }
	  else if(objno==2)	//rectangle
	  { setcolor(0);
	    rectangle(x1,y1,x2,y2);  }
		else if(objno==3)
		{
		triangle(x1,y1,x2,y2,15);
		}
	  else if(objno==4)
	  { setcolor(0);
	    radius=abs(sqrt(((y2-y1)*(y2-y1))+((x2-x1)*(x2-x1))));
	    circle(x1,y1,radius);
	  }
	  else
	  { }
	  */
	}

	if(mouseb==1 && flag==2)
	{
	  x2=out.x.cx;
	  y2=out.x.dx;

	  d_x=x2-x1;
	  d_y=y2-y1;
	  xc1[layerflag]+=d_x;
	  yc1[layerflag]+=d_y;
	  xc2[layerflag]+=d_x;
	  yc2[layerflag]+=d_y;

	  x1=x2;
	  y1=y2;
/*
	  if(op[layerflag]==1)
	  {  dda(xc1[layerflag],yc1[layerflag],xc2[layerflag],yc2[layerflag],15);  }
	  else if(op[layerflag]==2)
	  {  setcolor(15);
	     rectangle(xc1[layerflag],yc1[layerflag],xc2[layerflag],yc2[layerflag]); }
	else if(op[layerflag]==3)
	{
	triangle(xc1[layerflag],yc1[layerflag],xc2[layerflag],yc2[layerflag],15);
	}
	 else if(op[layerflag]==4)
	  { setcolor(15);
	    radius=abs(sqrt(((yc2[layerflag]-yc1[layerflag])*(yc2[layerflag]-yc1[layerflag]))+((xc2[layerflag]-xc1[layerflag])*(xc2[layerflag]-xc1[layerflag]))));
	    circle(xc1[layerflag],yc1[layerflag],radius);  }
	  else
	  { }
	  */
	}
	if(mouseb==0 && flag==2)
	{
	  d_x=x2-x1;
	  d_y=y2-y1;
	  matrixtrans(xc1[layerflag],yc1[layerflag],d_x,d_y,t);
	  xc1[layerflag]=t[0];
	  yc1[layerflag]=t[1];
	  matrixtrans(xc2[layerflag],yc2[layerflag],d_x,d_y,t);
	  xc2[layerflag]=t[0];
	  yc2[layerflag]=t[1];


	  if(op[layerflag]==1)
	  {  dda(xc1[layerflag],yc1[layerflag],xc2[layerflag],yc2[layerflag],2);
	  clr();
	  }
	  else if(op[layerflag]==2)
	  {  setcolor(2);
	     rectangle(xc1[layerflag],yc1[layerflag],xc2[layerflag],yc2[layerflag]);
		 clr();
		 }
		else if(op[layerflag]==3)
	{
	triangle(xc1[layerflag],yc1[layerflag],xc2[layerflag],yc2[layerflag],2);
	clr();
	}
	  else if(op[layerflag]==4)
	  {  setcolor(15);
	     radius=abs(sqrt(((y2-y1)*(y2-y1))+((x2-x1)*(x2-x1))));
	     circle(xc1[layerflag],yc1[layerflag],radius); 
clr();		 }
	  else
	  { }
	  flag=0;
	}

}
getch();
closegraph();

}

