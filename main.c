#include "mysparse.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char ** argv)
{

if (argc != 3) { printf ("ERROR: incorrect number of command line arguments\n"); return -1; }
int a = atoi (argv[1]);
int b = atoi (argv[2]);


/*GENERATE MATRIX*/
	if (a==0)
	{
		frandmat ("m010.txt",10000,10000,.01); //CHANGE ME
		return 0;
	}

/*COMPARISON OF ADDING METHODS*/
	else if (a == 1)
	{
		clock_t t = clock();
		double time_taken;
		int repeat = 30; //CHANGE ME

		for (int i=0; i<repeat; i++) {

			FILE * fp = fopen ("m010.txt", "r"); //CHANGE ME
			cs* T = cs_load (fp);
			fclose (fp);
			cs* A = cs_compress (T);
			cs_spfree (T);

			if (b == 1)
			{
				printf ("optimized: ");
				t = clock();
				if (!(diffshape(A,A)))
				{ mod(A,A); printf ("%d\n",A->nzmax); }
				t = clock() - t;
				cs_spfree (A);
			}
			else if (b == 2)
			{
				printf ("cs_add: ");
				t = clock();
				cs* B = cs_add (A,A,1,1);
				printf ("%d\n",B->nzmax);
				t = clock() - t;
				cs_spfree (B);
				cs_spfree (A);
			}
			else { printf ("ERROR: wrong command line argument\n"); cs_spfree (A); return -1; }

//			for (int i=0;i<A->nzmax;i++) { if (A->x[i] != 2) printf ("ERROR: incorrect sum\n"); }

			time_taken = ((double)t)/CLOCKS_PER_SEC;
			printf ("%f seconds to execute\n",time_taken);

			fp = fopen ("time.txt","a");
			fprintf (fp,"%f\n",time_taken);
			fclose (fp);
		}

		return 0;
	}


/*ADD_JAC AND SET_JAC TESTING*/
	else if (a == 2)
	{
		/*predefined matrices*/
		int r1[] =    {3,7,3,6,9,2,0,0,2};
		int c1[] =    {6,5,5,2,1,7,9,6,6};
		double v1[] = {1,1,1,1,1,1,1,1,1};
		int size1 = sizeof(r1)/sizeof(r1[0]);
	  	int r2[] =    {3,7,3,6,9,2,0,0,2};
		int c2[] =    {6,5,5,2,1,7,9,6,6};
		double v2[] = {1,1,1,1,1,1,1,1,1};
		int size2 = sizeof(r2)/sizeof(r2[0]);
		int m=10,n=10;

		if (b == 1) /*jac.c function testing*/
		{
			cs** jac_stor = alloc_jac (m,n);
			cs* mGy,*mGx;
			set_jac (jac_stor,Gy,r1,c1,v1,size1);
			add_jac (jac_stor,Gy,r2,c2,v2,size2);
			set_jac (jac_stor,Gx,r2,c2,v2,size2);

			mGy = acc_jac (jac_stor,Gy);
			mGx = acc_jac (jac_stor,Gx);
			print (mGy);
			print (mGx);
			free_jac (jac_stor);
		}
		else if (b == 2) /*matrix creation testing*/
		{
			cs* A = fcreate (m,n,r1,c1,v1,size1);
			cs* B = fcreate (m,n,r2,c2,v2,size2);
			A = add2 (A,B);
			print (A);
			cs_spfree (A);
			cs_spfree (B);
		}
		else { printf ("ERROR: wrong command line argument\n"); return -1; }
	}
	else { printf ("ERROR: wrong command line argument\n"); return -1; }
}



/*sample matrices*/
/*
	int r1[] =    {1,2,3, 0,2,4, 1,5, 0,1,3, 2,4, 0,5};
	int c1[] =    {0,0,0, 1,1,1, 3,3, 4,4,4, 5,5, 6,6};
	double v1[] = {4,7,10,1,8,12,5,14,2,6,11,9,13,3,15};
	int size1 = sizeof(r1)/sizeof(r1[0]);
        int r2[] =    {1,2,1,1};
	int c2[] =    {0,1,3,4};
	double v2[] = {100,100,100,100};
	int size2 = sizeof(r2)/sizeof(r2[0]);


	int r1[] =    {1,2,3, 0,2,4, 1,5, 0,1,3, 2,4, 0,5};
	int c1[] =    {0,0,0, 1,1,1, 3,3, 4,4,4, 5,5, 6,6};
	double v1[] = {4,7,10,1,8,12,5,14,2,6,11,9,13,3,15};
	int size1 = sizeof(r1)/sizeof(r1[0]);
        int r2[] =    {2,3,1,5,2,3,4,3,2,1,6,3,6,2,1};
	int c2[] =    {0,0,1,1,1,1,2,2,2,3,4,5,5,6,6};
	double v2[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	int size2 = sizeof(r2)/sizeof(r2[0]);
        int r3[] =    {1,2,0,4,1,2,3,2,1,0,5,2,6,1,0};
	int c3[] =    {0,0,1,1,1,1,2,2,2,3,4,5,5,6,6};
	double v3[] = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
*/