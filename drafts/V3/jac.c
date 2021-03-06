#include "cs.h"
#include "mysparse.h"

int NUM_JAC = 8;

/*allocates array of pointers to jacobian matrices*/
cs** alloc_jac (int m, int n, int triplet)
{
	cs* Fx,*Fy,*Gx,*Gy,*Fx0,*Fy0,*Gx0,*Gy0;
	cs** jac_stor = malloc (NUM_JAC*sizeof(cs*));
	Fx = create (m,n,triplet);
	Fy = create (m,n,triplet);
	Gx = create (m,n,triplet);
	Gy = create (m,n,triplet);
	Fx0 = create (m,n,triplet);
	Fy0 = create (m,n,triplet);
	Gx0 = create (m,n,triplet);
	Gy0 = create (m,n,triplet);

	jac_stor[0]=Fx; jac_stor[1]=Fy; jac_stor[2]=Gx; jac_stor[3]=Gy;
	jac_stor[4]=Fx0; jac_stor[5]=Fy0; jac_stor[6]=Gx0; jac_stor[7]=Gy0;
	return jac_stor;
}

/*frees array of pointers to jacobian matrices*/
int free_jac (cs** jac_stor)
{
	int k;
	for (k=0;k<NUM_JAC;k++)
	{
		if (!(jac_stor[k])) return 1;
		cs_spfree (jac_stor[k]);
	}
	free (jac_stor);
	return 0;
}

/*returns jacobian matrix given jac_name*/
cs* acc_jac (cs** jac_stor, jac_name s)
{
	return jac_stor[s];
}


/*adds values to empty jacobian matrix*/
int add_jac (cs** jac_stor, jac_name s, int* r, int* c, double* v, int size)
{
	int k; cs* T = acc_jac (jac_stor,s);
	if (errarr (T,r,c,size)) return 3; /*return 3 if outside bounds of matrix*/
	for (k=0;k<size;k++) if (add (T,r[k],c[k],v[k])) return 2; /*return 2 if add fails*/
	return 0;
}

/*sets values of full jacobian matrix*/
int set_jac (cs** jac_stor, jac_name s, int* r, int* c, double* v, int size)
{
	int k; cs* T = acc_jac (jac_stor,s);
	if (errarr (T,r,c,size)) return 3; /*return 3 if outside bounds of matrix*/
	if (clear(T)) return 1; /*return 2 if clear fails*/
	for (k=0;k<size;k++) if (add (T,r[k],c[k],v[k])) return 2; /*return 2 if add fails*/
	return 0;
}
