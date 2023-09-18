//-----------------------------------------------------------------------------------------------------------------------------
//  The Binary Tree Demo 
//  by Programfanny
//  2023-09-18
//-----------------------------------------------------------------------------------------------------------------------------
/*
Compile: gcc wintree.c -owintree.exe -mwindows
Show a bitree structure in a window
*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define QueueMax 10000

struct TreeNode {
	int val;
	int count;
	int s,d;//d=layer, s=number in complete tree of the layer
	struct TreeNode* pleft;
	struct TreeNode* pright;
};
typedef struct TreeNode BiTree, *pBiTree;
typedef struct{
	pBiTree data[QueueMax];
	int head;
	int rear;
	int len;
}Queue;

Queue InitQueue();
int IsEmptyQueue(Queue seq);
int IsFullQueue(Queue seq);
void PushQueue(Queue *seq, pBiTree T);
void PopQueue(Queue *seq, pBiTree *T);
pBiTree AddNode(int value, pBiTree pnode);
void avlTree(pBiTree pnode);
pBiTree CreateNode(int value);
void destroyTree(pBiTree pnode);
void drawBiTree(HWND hwnd,pBiTree root);
void drawTree(HWND hwnd);
int getTreeDepth(pBiTree pnode);
void initialTree(pBiTree root,int *nums,int *numsSize);
void inorderDFS(pBiTree proot, int* arr, int* count);
int* inorderTraversal(pBiTree root, int* returnSize);
void innerDrawTree(pBiTree pnode, pBiTree *arr, int *count, int xp, int dep);
void innerList(pBiTree pnode, int* arr, int* count);
int *innerListNodes(pBiTree root, int* returnSize);
int *layerListNodes(pBiTree pnode,int *returnSize);
void postList(pBiTree pnode, int* arr, int* count);
int *postListNodes(pBiTree root, int* returnSize);
void preList(pBiTree pnode, int* arr, int* count);
int *preListNodes(pBiTree root, int* returnSize);

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){
	static TCHAR szAppName[] = TEXT ("HelloWin") ;
	HWND         hwnd ;
	MSG          msg ;
	WNDCLASS     wndclass ;

	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;

	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("This program requires Windows NT!"), szAppName, MB_ICONERROR) ;
		return 0 ;
	}
	hwnd = CreateWindow (szAppName,                 	// window class name
						TEXT ("The Binary Tree Demo"), 	// window caption
						WS_OVERLAPPEDWINDOW,        	// window style
						CW_USEDEFAULT,              	// initial x position
						CW_USEDEFAULT,              	// initial y position
						CW_USEDEFAULT,              	// initial x size
						CW_USEDEFAULT,              	// initial y size
						NULL,                       	// parent window handle
						NULL,                       	// window menu handle
						hInstance,                  	// program instance handle
						NULL) ;                     	// creation parameters
	 
	 ShowWindow (hwnd, iCmdShow) ;
	 UpdateWindow (hwnd) ;
	 
	 while (GetMessage (&msg, NULL, 0, 0))
	 {
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	 }
	 return msg.wParam ;
}
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	HDC         hdc ;
	PAINTSTRUCT ps ;
	static RECT rect ;
	static pBiTree root;
	static int returnSizeA = 0,returnSizeB = 0,returnSizeC = 0,returnSizeD = 0,returnSizeE = 0;
	static int *arrA,*arrB,*arrC,*arrD,*arrE;	
	static int *nums,numsSize;
	char buf[100];
	switch (message)
	{
	case WM_CREATE:
		srand(time(0));
		numsSize = rand()%28+3;
		nums = (int*)malloc(numsSize * sizeof(int));
		for(int i=0;i<numsSize;i++)nums[i]=rand()%50;
		root = CreateNode(nums[0]);     
		for(int i=1;i<numsSize;i++){
			AddNode(nums[i], root);
		}
		return 0 ;
	case WM_PAINT:
		hdc = BeginPaint (hwnd, &ps) ;
		GetClientRect (hwnd, &rect) ;
		for(int i=0;i<numsSize;i++){
			sprintf(buf,"%d ",nums[i]);
			TextOut(hdc,10,20+i*20,buf,strlen(buf));
		}
		for(int i=0;i<returnSizeA;i++){
			sprintf(buf,"%d ",arrA[i]);
			TextOut(hdc,60,20+i*20,buf,strlen(buf));
		}
	
		for(int i=0;i<returnSizeB;i++){
			sprintf(buf,"%d ",arrB[i]);
			TextOut(hdc,110,20+i*20,buf,strlen(buf));
		}

		for(int i=0;i<returnSizeC;i++){
			sprintf(buf,"%d ",arrC[i]);
			TextOut(hdc,160,20+i*20,buf,strlen(buf));
		}
	
		for(int i=0;i<returnSizeD;i++){
			sprintf(buf,"%d ",arrD[i]);
			TextOut(hdc,210,20+i*20,buf,strlen(buf));
		}
	
		for(int i=0;i<returnSizeE;i++){
			sprintf(buf,"%d ",arrE[2*i]);
			TextOut(hdc,260,20+i*20,buf,strlen(buf));
		}
		for(int i=0;i<returnSizeE;i++){
			sprintf(buf,"[%d] ",arrE[2*i+1]);
			TextOut(hdc,290,20+i*20,buf,strlen(buf));
		}	
		drawTree(hwnd);
		drawBiTree(hwnd, root);

		EndPaint (hwnd, &ps);
		return 0 ;
	case WM_TIMER:
		return 0;	
	case WM_KEYDOWN:
		switch(wParam){
			case VK_ESCAPE:
				PostMessage( hwnd,WM_DESTROY,0,0 );
				break;
			case VK_F2:
				arrA = preListNodes(root,&returnSizeA);
				arrB = innerListNodes(root,&returnSizeB);
				arrC = postListNodes(root,&returnSizeC);
				arrD = inorderTraversal(root,&returnSizeD);
				arrE = layerListNodes(root,&returnSizeE);
				//arrE = restoreTree(root,&returnSizeE);
				break;
			case VK_RETURN:
				if(nums)free(nums);
				if(root){
					if(arrA)free(arrA);
					if(arrB)free(arrB);
					if(arrC)free(arrC);
					if(arrD)free(arrD);
					if(arrE)free(arrE);
					destroyTree(root);
				}
				root=NULL;
				nums = (int*)malloc(numsSize * sizeof(int));
				for(int i=0;i<numsSize;i++)nums[i]=rand()%50;
				root = CreateNode(nums[0]);     
				for(int i=1;i<numsSize;i++){
					AddNode(nums[i], root);
				}
			break;		
		}
		InvalidateRect(hwnd,&rect,TRUE);
		return 0;	
		  
	case WM_DESTROY:
		free(nums);
		free(arrA);free(arrB);free(arrC);free(arrD);free(arrE);
		destroyTree(root);
		PostQuitMessage (0) ;
		return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}
Queue InitQueue(){
	Queue seq;
	for(int i = 0; i < QueueMax; i++) {
		seq.data[i] = NULL;
	}
	seq.head = 0;
	seq.rear = -1;
	seq.len = 0;
	return seq;
}
int IsEmptyQueue(Queue seq){
	if (seq.len == 0) {
		return 1;
	}
	return 0;
}
int IsFullQueue(Queue seq){ 
	if (seq.len == QueueMax) {
		return 1;
	}
	return 0;
}
void PushQueue(Queue *seq, pBiTree T){ 
	if (IsFullQueue(*seq)) { 
		printf("ErrorFull");
		return;
	}
	seq->rear = (seq->rear + 1) % QueueMax;
	seq->len++;
	seq->data[seq->rear] = T;
}
void PopQueue(Queue *seq, pBiTree *T){ 
	if (IsEmptyQueue(*seq)) {    
		printf("ErrorEmpty");
		return;
	}
	seq->head = (seq->head + 1) % QueueMax;
	*T = seq->data[seq->head];
	seq->len--;
}
pBiTree AddNode(int value, pBiTree pnode){
	if(pnode == NULL)return CreateNode(value);
	//if(value == pnode->val){pnode->count++; return pnode;}
	if(value <= pnode->val){
		if(pnode->pleft==NULL){
			pnode->pleft = CreateNode(value);
			return pnode->pleft;
		}else{
			return AddNode(value,pnode->pleft);
		}
	}else{
		if(pnode->pright == NULL){
			pnode->pright=CreateNode(value);
			return pnode->pright;
		}else{
			return AddNode(value,pnode->pright);
		}
	}
}
void avlTree(pBiTree pnode){

}
pBiTree CreateNode(int value){
	pBiTree pnode =(pBiTree)malloc(sizeof(BiTree));
	pnode->val = value;pnode->count=1;
	pnode->pleft = pnode->pright = NULL;
	return pnode;
}
void destroyTree(pBiTree pnode){
	if (pnode == NULL) return;
	destroyTree(pnode->pleft);
	destroyTree(pnode->pright);
	printf("%d ", pnode->val);
	free(pnode);
	pnode=NULL;
}
void drawBiTree(HWND hwnd,pBiTree root){
	// (s,d)--->(x,y)
	// 6 layers
	char buf[100];
	RECT rect;
	HPEN hPen = CreatePen(PS_SOLID,3,RGB(255,0,0));
	GetClientRect(hwnd, &rect);
	HDC hdc = GetDC(hwnd);
	SelectObject(hdc,hPen);
	float x, y, dx, dy;
	int ss,count=0,s,d;
	pBiTree *arr=malloc(100*sizeof(pBiTree));
	dy=(rect.bottom - rect.top)/8.0;
	innerDrawTree(root,arr,&count,1,0);
	//for(int i=0;i<count;i++){
	//	if(arr[i]){
	//		sprintf(buf,"%d %d %d",arr[i]->val,arr[i]->s,arr[i]->d);
	//		TextOut(hdc,210,10+i*20,buf,strlen(buf));
	//	}
	//}	
	for(int i=0;i<count;i++){
		if((arr[i])->d<6){
			s=2*arr[i]->s+1;
			d=arr[i]->d+1;
			ss=1<<d;
			dx=(rect.right-rect.left)*1.0/ss;

			x=s*dx;y=20+d*dy;

			Ellipse(hdc,x-15,y-15,x+15,y+15);
			sprintf(buf,"%d",arr[i]->val);
			TextOut(hdc,x-10,y-10,buf,strlen(buf));
			if(arr[i]->d>0){
				MoveToEx(hdc,x,y-15,NULL);
				LineTo(hdc,(2*(2*(s/4+1)-1))*dx,y-dy+15);
			}
		}
	}
	
	ReleaseDC(hwnd,hdc);
	DeleteObject(hPen);
}
void drawTree(HWND hwnd){
	char buf[100];
	RECT rect;
	HPEN hPen = CreatePen(PS_SOLID,1,RGB(192,240,255));
	GetClientRect(hwnd,&rect);
	HDC hdc = GetDC(hwnd);
	SelectObject(hdc,hPen);
	float x, y, dx, dy;
	int ss;
	dy=(rect.bottom-rect.top)/8.0;
	for (int d=0;d<6;d++){
		int dep=6-d;
		y=20+dy*dep;
		ss=1<<dep;
		dx=(rect.right-rect.left)*1.0/ss;
		for(int s=1;s<ss;s+=2){
			x=s*dx;
			Ellipse(hdc,x-15,y-15,x+15,y+15);
			if(dep-1>0){
				MoveToEx(hdc,x,y-15,NULL);
				LineTo(hdc,(2*(2*(s/4+1)-1))*dx,y-dy+15);
			}
		}
	}
	ReleaseDC(hwnd,hdc);
}
int getTreeDepth(pBiTree pnode){
	int rightdep=0;
	int leftdep=0;
	if(pnode==NULL)return -1;
	if(pnode->pleft!=NULL)
		leftdep=getTreeDepth(pnode->pleft);
	else
		leftdep=-1;
	if(pnode->pright!=NULL)
		rightdep=getTreeDepth(pnode->pright);
	else
		rightdep=-1;
	return (rightdep>leftdep)?rightdep+1:leftdep+1;
}
void initialTree(pBiTree root,int *nums,int *numsSize){
	int size = rand()%28+3;
	nums = (int*)malloc(size * sizeof(int));
	for(int i=0;i<size;i++)nums[i]=rand()%50;
	root = CreateNode(nums[0]);     
	for(int i=1;i<size;i++){
		AddNode(nums[i], root);
	}
	*numsSize = size;
}
void inorderDFS(pBiTree proot, int* arr, int* count) {
	if(proot){
		inorderDFS(proot->pleft, arr, count);
		arr[(*count)++] = proot->val;
		inorderDFS(proot->pright, arr, count);
	}
}
int* inorderTraversal(pBiTree root, int* returnSize) {
	int* arr = (int *)malloc(sizeof(int) * 100);
	int count = 0;
	inorderDFS(root, arr, &count);
	*returnSize = count;
	return arr;
}
void innerDrawTree(pBiTree pnode, pBiTree *arr, int *count,int xp,int dep){
	if(pnode){
		innerDrawTree(pnode->pleft,arr,count,xp*2,dep+1);
		arr[*count]=pnode;
		pnode->s = xp-(1<<dep);
		pnode->d = dep;
		(*count)++;		
		innerDrawTree(pnode->pright,arr,count, xp*2+1,dep+1);
	}
}
void innerList(pBiTree pnode, int* arr, int* count){
	if(pnode){
		innerList(pnode->pleft, arr, count);
		arr[(*count)++] = pnode->val;
		innerList(pnode->pright, arr, count);
	}
}
int *innerListNodes(pBiTree root, int* returnSize) {
	int* arr = (int *)malloc(sizeof(int) * 100);
	int count = 0;
	innerList(root, arr, &count);
	*returnSize = count;
	return arr;
}
int * layerListNodes(pBiTree pnode,int *returnSize){
	int* arr = (int *)malloc(sizeof(int) * 200);
	int count = 0;
	Queue seq;
	seq = InitQueue();
	pBiTree tmp;
	tmp = pnode;
	int len;
	PushQueue(&seq,tmp);
	int level=0;
	while(!IsEmptyQueue(seq)){
		len = seq.len;
		for(int i=0;i<len;i++){
			arr[count*2]=tmp->val;
			arr[count*2+1]=level;
			count++;
			if(tmp->pleft)PushQueue(&seq,tmp->pleft);
			if(tmp->pright)PushQueue(&seq,tmp->pright); 
			PopQueue(&seq,&tmp);
		}
		level++; 
	}
	*returnSize=count;
	return arr;
}
void postList(pBiTree pnode, int* arr, int* count){
	if(pnode){
		postList(pnode->pleft, arr, count);
		postList(pnode->pright, arr, count);
		arr[(*count)++] = pnode->val;
	}
}
int *postListNodes(pBiTree root, int* returnSize) {
	int* arr = (int *)malloc(sizeof(int) * 100);
	int count = 0;
	postList(root, arr, &count);
	*returnSize = count;
	return arr;
}
void preList(pBiTree pnode, int* arr, int* count){
	if(pnode){
		arr[(*count)++] = pnode->val;
		preList(pnode->pleft, arr, count);
		preList(pnode->pright, arr, count);
	}
}
int *preListNodes(pBiTree root, int* returnSize) {
	int* arr = (int *)malloc(sizeof(int) * 100);
	int count = 0;
	preList(root, arr, &count);
	*returnSize = count;
	return arr;
}

