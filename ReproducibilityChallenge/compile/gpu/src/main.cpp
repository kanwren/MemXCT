#include "vars.h"

double ftime = 0;
double btime = 0;
double fktime = 0;
double frtime = 0;
double bktime = 0;
double brtime = 0;
double aftime = 0;
double abtime = 0;

int numx; //X SIZE OF DOMAIN
int numy; //Y SIZE OF DOMAIN
int numthe; //NUMBER OF THETAS
int numrho; //NUMBER OF RHOS

float xstart; //X START OF DOMAIN
float ystart; //Y START OF DOMAIN
float pixsize; //PIXEL SIZE
float rhostart; //RHO START
float raylength; //RAYLENGTH

char *sinfile; //SINOGRAM FILE
char *thefile; //THETA FILE
char *outfile; //OUTPUT FILE

int numiter;

int spatindexing;
int specindexing;

int spatsize; //SPATIAL TILE SIZE
int specsize; //SPECTRAL TILE SIZE
int numxtile; //NUMBER OF X TILES
int numytile; //NUMBER OF Y TILES
int numttile; //NUMBER OF THETA TILES
int numrtile; //NUMBER OF RHO TILES
int numpix; //NUMBER OF PIXELS (EXTENDED)
int numray; //NUMBER OF RAYS (EXTENDED)
int numspattile; //NUMBER OF SPATIAL TILES
int numspectile; //NUMBER OF SPECTRAL TILES

float *raypart;
float *raybuff;

int raynuminc;
int raynumout;
int mynumray;
int mynumpix;

int *raysendstart;
int *rayrecvstart;
int *raysendcount;
int *rayrecvcount;

int *rayraystart;
int *rayrayind;

int *rayrecvlist;

int proj_rownztot;
long proj_rownzall;
int *proj_rowdispl;
int *proj_rowindex;
float *proj_rowvalue;
int proj_blocksize;
int proj_numblocks;
int proj_blocknztot;
long proj_blocknzall;
int *proj_blockdispl;
int *proj_blockindex;
float *proj_blockvalue;
int proj_buffsize;
int *proj_buffdispl;
int proj_buffnztot;
long proj_buffnzall;
int *proj_buffmap;
short *proj_buffindex;
float *proj_buffvalue;
int *back_rowdispl;
int *back_rowindex;
float *back_rowvalue;
int back_blocksize;
int back_numblocks;
int back_blocknztot;
long back_blocknzall;
int *back_blockdispl;
int *back_blockindex;
float *back_blockvalue;
int back_buffsize;
int *back_buffdispl;
int back_buffnztot;
long back_buffnzall;
int *back_buffmap;
short *back_buffindex;
float *back_buffvalue;

int numproj;
int numback;

int main(int argc, char** argv){

  int numproc;
  int myid;
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numproc);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);

  MPI_Barrier(MPI_COMM_WORLD);
  double timetot = MPI_Wtime();

  int numthreads;
  #pragma omp parallel
  if(omp_get_thread_num()==0)numthreads=omp_get_num_threads();

  //SCANNING GEOMETRY DATA
  char *chartemp;
  chartemp = getenv("NUMTHE");
  numthe = atoi(chartemp);
  chartemp = getenv("NUMRHO");
  numrho = atoi(chartemp);
  //chartemp = getenv("NUMX");
  numx = numrho;//atoi(chartemp);
  //chartemp = getenv("NUMY");
  numy = numrho;//atoi(chartemp);

  //chartemp = getenv("XSTART");
  float xstart = -numrho/2.0;//atof(chartemp);
  //chartemp = getenv("YSTART");
  float ystart = xstart;//atof(chartemp);
  chartemp = getenv("PIXSIZE");
  pixsize = atof(chartemp);
  //chartemp = getenv("RHOSTART");
  rhostart = xstart;//atof(chartemp);
  //chartemp = getenv("RAYLENGTH");
  raylength = 2*numx;// atof(chartemp);

  //chartemp = getenv("SPATINDEXING");
  spatindexing = 5;//atoi(chartemp);
  //chartemp = getenv("SPECINDEXING");
  specindexing = 5;//atoi(chartemp);

  chartemp = getenv("NUMITER");
  numiter = atoi(chartemp);

  chartemp = getenv("SPATSIZE");
  spatsize = atof(chartemp);
  chartemp = getenv("SPECSIZE");
  specsize = atof(chartemp);

  chartemp = getenv("PROJBLOCK");
  proj_blocksize = atoi(chartemp);
  chartemp = getenv("BACKBLOCK");
  back_blocksize = atoi(chartemp);
  chartemp = getenv("PROJBUFF");
  proj_buffsize = atoi(chartemp);
  chartemp = getenv("BACKBUFF");
  back_buffsize = atoi(chartemp);

  sinfile = getenv("SINFILE");
  thefile = getenv("THEFILE");
  outfile = getenv("OUTFILE");

  //FIND NUMBER OF TILES
  numxtile = numx/spatsize;
  if(numx%spatsize)numxtile++;
  numytile = numy/spatsize;
  if(numy%spatsize)numytile++;
  numrtile = numrho/specsize;
  if(numrho%specsize)numrtile++;
  numttile = numthe/specsize;
  if(numthe%specsize)numttile++;
  numspattile = numxtile*numytile;
  numspectile = numrtile*numttile;
  numpix = numspattile*pow(spatsize,2);
  numray = numspectile*pow(specsize,2);
  //PRINT DATA
  if(myid==0){
    printf("NUM. THETA             : %d\n",numthe);
    printf("NUM. RHO               : %d\n",numrho);
    printf("NUM. X PIXELS          : %d\n",numx);
    printf("NUM. Y PIXELS          : %d\n",numy);
    printf("\n");
    printf("NUM. OF PIXELS         : %d\n",numx*numy);
    printf("NUM. OF RAYS           : %d\n",numrho*numthe);
    printf("\n");
    printf("NUM. ITERATIONS        : %d\n",numiter);
    printf("\n");
    printf("SPATIAL TILE SIZE      : %d\n",spatsize);
    printf("SPECTRAL TILE SIZE     : %d\n",specsize);
    printf("\n");
    printf("NUMBER OF X TILES      : %d\n",numxtile);
    printf("NUMBER OF Y TILES      : %d\n",numytile);
    printf("NUM. OF THETA TILES    : %d\n",numttile);
    printf("NUM. OF RHO TILES      : %d\n",numrtile);
    printf("\n");
    printf("NUM. SPATIAL TILES     : %d\n",numspattile);
    printf("NUM. SPECTRAL TILES    : %d\n",numspectile);
    printf("\n");
    printf("NUM. OF X PIXELS (EXT) : %d\n",numxtile*spatsize);
    printf("NUM. OF Y PIXELS (EXT) : %d\n",numytile*spatsize);
    printf("NUM. OF ANGLES (EXT)   : %d\n",numttile*specsize);
    printf("NUM. OF RHOS (EXT)     : %d\n",numrtile*specsize);
    printf("\n");
    printf("NUM. OF PIXELS (EXT)   : %d\n",numpix);
    printf("NUM. OF RAYS (EXT)     : %d\n",numray);
    printf("\n");
    printf("NUMBER OF PROCESSES    : %d\n",numproc);
    printf("NUMBER OF THRD./PROC.  : %d\n",numthreads);
    printf("\n");
    printf("INTEGER: %d, FLOAT: %d, LONG: %d, SHORT: %d, POINTER: %d\n",(int)sizeof(int),(int)sizeof(float),(int)sizeof(long),(int)sizeof(short),(int)sizeof(complex<float>*));
    printf("APPROX. MEMORY     TOTAL: %e GB\n",(numthe*numrho)/1024.0/1024.0/1024.0*numrho*16.0*1.5);
    printf("APPROX. MEMORY PER PROC.: %e GB\n",(numthe*numrho)/1024.0/1024.0/1024.0*numrho*16.0/numproc*1.5);
    printf("\n");
    printf("X START       : %e\n",xstart);
    printf("Y START       : %e\n",ystart);
    printf("PIXEL SIZE    : %e\n",pixsize);
    printf("RHO START     : %e\n",rhostart);
    printf("RAY LENGTH    : %e\n",raylength);
    printf("\n");
    printf("SPATIAL INDEXING       : %d\n",spatindexing);
    printf("SPECTRAL INDEXING      : %d\n",specindexing);
    printf(" 1: CARTESIAN, NATURAL\n 2: CARTESIAN, TRANSPOSED\n 3: MORTON, NATURAL\n 4: MORTON, TRANSPOSED\n");
    printf("PROJECTION BLOCK SIZE      : %d\n",proj_blocksize);
    printf("BACKPROJECTION BLOCK SIZE  : %d\n",back_blocksize);
    printf("PROJECTION BUFFER SIZE      : %d KB\n",proj_buffsize);
    printf("BACKPROJECTION BUFFER SIZE  : %d KB\n",back_buffsize);
    printf("\n");
    printf("SINOGRAM FILE : %s\n",sinfile);
    printf("   THETA FILE : %s\n",thefile);
    printf("  OUTPUT FILE : %s\n",outfile);
    printf("\n");
    printf("\n");
  }
  proj_buffsize = proj_buffsize*1024/4;
  back_buffsize = back_buffsize*1024/4;

  if(myid==0)printf("PLACE TILES\n");
  //PLACE SPATIAL TILES
  int lspat = numxtile;
  if(numytile > lspat)lspat = numytile;
  int spatlevel = 0;
  while(true){
    if(lspat<=pow(2,spatlevel))break;
    spatlevel++;
  }
  int lspatdim = pow(2,spatlevel);
  if(myid==0)printf("lspat %d lspatdim %d\n",lspat,lspatdim);
  complex<float> *spatlltemp = new complex<float>[lspatdim*lspatdim];
  #pragma omp parallel for
  for(int lspat = 0; lspat < lspatdim*lspatdim; lspat++)
    spatlltemp[lspat].real(xstart-1);
  #pragma omp parallel for
  for(int spat = 0; spat < numspattile; spat++){
    int ytile = spat/numxtile;
    int xtile = spat%numxtile;
    int ind = 0;
      //ind = ytile*numxtile+xtile;
      //ind = encode(xtile,ytile);
      ind = xy2d (numxtile,xtile,ytile);
    float x = xstart+xtile*spatsize*pixsize;
    float y = ystart+ytile*spatsize*pixsize;
    spatlltemp[ind]=complex<float>(x,y);
  }
  complex<float> *spatll = new complex<float>[numspattile];
  int spatcount = 0;
  for(int lspat = 0; lspat < lspatdim*lspatdim; lspat++)
    if(spatlltemp[lspat].real()>xstart-0.5){
      spatll[spatcount] = spatlltemp[lspat];
      spatcount++;
    }
  delete[] spatlltemp;
  //PLACE SPECTRAL TILES
  int lspec = numrtile;
  if(numttile > lspec)lspec = numttile;
  int speclevel = 0;
  while(true){
    if(lspec<=pow(2,speclevel))break;
    speclevel++;
  }
  int lspecdim = pow(2,speclevel);
  if(myid==0)printf("lspec %d lspecdim %d\n",lspec,lspecdim);
  complex<float> *speclltemp = new complex<float>[lspecdim*lspecdim];
  #pragma omp parallel for
  for(int lspec = 0; lspec < lspecdim*lspecdim; lspec++)
    speclltemp[lspec].real(rhostart-1);
  #pragma omp parallel for
  for(int spec = 0; spec < numspectile; spec++){
    int thetile = spec/numrtile;
    int rhotile = spec%numrtile;
    int ind = 0;
      //ind = thetile*numrtile+rhotile;
      //ind = encode(rhotile,thetile);
      ind = xy2d(lspecdim,rhotile,thetile);
    float rho = rhostart+rhotile*specsize;
    float the = thetile*specsize*M_PI/numthe;
    speclltemp[ind]=complex<float>(rho,the);
  }
  complex<float> *specll = new complex<float>[numspectile];
  int speccount = 0;
  for(int lspec = 0; lspec < lspecdim*lspecdim; lspec++)
    if(speclltemp[lspec].real()>rhostart-0.5){
      specll[speccount] = speclltemp[lspec];
      speccount++;
    }
  delete[] speclltemp;
  if(myid==0)printf("MPI PARTITIONING\n");
  int *numspats = new int[numproc];
  int *numspecs = new int[numproc];
  int *spatstart = new int[numproc];
  int *specstart = new int[numproc];
  int *numpixs = new int[numproc];
  int *numrays = new int[numproc];
  int *pixstart = new int[numproc];
  int *raystart = new int[numproc];
  int myspattile = numspattile/numproc;
  if(myid < numspattile-myspattile*numproc)myspattile++;
  int myspectile = numspectile/numproc;
  if(myid < numspectile-myspectile*numproc)myspectile++;
  MPI_Allgather(&myspattile,1,MPI_INTEGER,numspats,1,MPI_INTEGER,MPI_COMM_WORLD);
  MPI_Allgather(&myspectile,1,MPI_INTEGER,numspecs,1,MPI_INTEGER,MPI_COMM_WORLD);
  spatstart[0] = 0;
  specstart[0] = 0;
  for(int p = 1; p < numproc; p++){
    spatstart[p] = spatstart[p-1] + numspats[p-1];
    specstart[p] = specstart[p-1] + numspecs[p-1];
  }
  for(int p = 0; p < numproc; p++){
    numpixs[p] = numspats[p]*spatsize*spatsize;
    numrays[p] = numspecs[p]*specsize*specsize;
  }
  pixstart[0] = 0;
  raystart[0] = 0;
  for(int p = 1; p < numproc; p++){
    pixstart[p] = pixstart[p-1] + numpixs[p-1];
    raystart[p] = raystart[p-1] + numrays[p-1];
  }
  mynumpix = numpixs[myid];
  mynumray = numrays[myid];
  int maxnumpix = 0;
  int maxnumray = 0;
  for(int p = 0; p < numproc; p++){
    if(numpixs[p]>maxnumpix)maxnumpix=numpixs[p];
    if(numrays[p]>maxnumray)maxnumray=numrays[p];
  }
  if(myid==0){
    for(int p = 0; p < numproc; p++)
      printf("proc: %d numspats: %d numpixs: %d numspecs: %d numrays: %d\n",p,numspats[p],numpixs[p],numspecs[p],numrays[p]);
    printf("maxnumpix: %d maxnumray: %d\n",maxnumpix,maxnumray);
  }
  if(myid==0)printf("FILL PIXELS AND RAYS\n");
  //PLACE PIXELS
  complex<float> *pixcoor = new complex<float>[mynumpix];
  int *pixglobalind = new int[mynumpix];
  #pragma omp parallel for
  for(int pix = 0; pix < mynumpix; pix++){
    int tile = pix/(spatsize*spatsize);
    int pixloc = pix%(spatsize*spatsize);
    int pixlocy = pixloc/spatsize;
    int pixlocx = pixloc%spatsize;
    int ind = 0;
    if(spatindexing==1)
      ind = pixlocy*spatsize+pixlocx;
    if(spatindexing==2)
      ind = pixlocx*spatsize+pixlocy;
    if(spatindexing==3)
      ind = encode(pixlocx,pixlocy);
    if(spatindexing==4)
      ind = encode(pixlocy,pixlocx);
    if(spatindexing==5)
      //ind = encode(pixlocx,pixlocy);
      ind = xy2d(spatsize,pixlocx,pixlocy);
    ind = ind + tile*spatsize*spatsize;
    float x = spatll[spatstart[myid]+tile].real()+pixsize/2+pixlocx*pixsize;
    float y = spatll[spatstart[myid]+tile].imag()+pixsize/2+pixlocy*pixsize;
    pixcoor[ind] = complex<float>(x,y);
    //GLOBAL SPATIAL INDEX (EXTENDED)
    int xglobalind = (int)((x-xstart)/pixsize);
    int yglobalind = (int)((y-ystart)/pixsize);
    pixglobalind[ind] = yglobalind*numxtile*spatsize+xglobalind;
  }

  /*float *mestheta = new float[numthe];
  if(myid==0)printf("INPUT THETA DATA\n");
  FILE *thetaf = fopen("brain-2x-5000-5001.1s.theta.4501.data","rb");
  fread(mestheta,sizeof(float),numthe,thetaf);
  fclose(thetaf);*/

  float *mestheta = new float[numthe];
  if(myid==0)printf("INPUT THETA DATA\n");
  FILE *thetaf = fopen(thefile,"rb");
  fread(mestheta,sizeof(float),numthe,thetaf);
  fclose(thetaf);

  //PLACE RAYS
  complex<float> *raycoor = new complex<float>[mynumray];
  int *rayglobalind = new int[mynumray];
  int *raymesind = new int[mynumray];
  #pragma omp parallel for
  for(int ray = 0; ray < mynumray; ray++){
    int tile = ray/(specsize*specsize);
    int rayloc = ray%(specsize*specsize);
    int raylocthe = rayloc/specsize;
    int raylocrho = rayloc%specsize;
    int ind = 0;
    if(specindexing==1)
      ind = raylocthe*specsize+raylocrho;
    if(specindexing==2)
      ind = raylocrho*specsize+raylocthe;
    if(specindexing==3)
      ind = encode(raylocrho,raylocthe);
    if(specindexing==4)
      ind = encode(raylocthe,raylocrho);
    if(specindexing==5)
      //ind = encode(raylocrho,raylocthe);
      ind = xy2d(specsize,raylocrho,raylocthe);
    ind = ind + tile*specsize*specsize;
    float rho = specll[specstart[myid]+tile].real()+0.5+raylocrho;
    float the = specll[specstart[myid]+tile].imag()+raylocthe*M_PI/numthe;
    //GLOBAL SPECTRAL INDEX (EXTENDED)
    int rhoglobalind = (int)(rho-rhostart);
    int theglobalind = (int)((the+(M_PI/numthe)/2)/(M_PI/numthe));
    rayglobalind[ind] = theglobalind*numrtile*specsize+rhoglobalind;
    if(theglobalind<numthe && rhoglobalind<numrho){
      raymesind[ind] = theglobalind*numrho+rhoglobalind;
      raycoor[ind] = complex<float>(rho,mestheta[theglobalind]);
      //raycoor[ind] = complex<float>(rho,the);
    }
    else{
      raycoor[ind].real(5*raylength);
      raymesind[ind] = -1;
    }
  }
  //delete[] mestheta;
  delete[] specll;
  if(myid==0)printf("DOMAIN PARTITIONING\n");
  rayrecvcount = new int[numproc];
  raysendcount = new int[numproc];
  rayrecvstart = new int[numproc];
  raysendstart = new int[numproc];
  float *lengthtemp = new float[mynumray];
  int **rayrecvtemp = new int*[numproc];
  for(int p = 0; p < numproc; p++){
    #pragma omp parallel for
    for(int k = 0; k < mynumray; k++){
      lengthtemp[k] = 0;
      float rho = raycoor[k].real();
      float theta = raycoor[k].imag();
      for(int tile = spatstart[p]; tile < spatstart[p]+numspats[p]; tile++){
        float domain[4];
        domain[0]=spatll[tile].real();
        domain[1]=domain[0]+spatsize*pixsize;
        domain[2]=spatll[tile].imag();
        domain[3]=domain[2]+spatsize*pixsize;
        //REMOVE SPATIAL EDGE CONDITION
        if(domain[1] > xstart+numx*pixsize)domain[1]=xstart+numx*pixsize;
        if(domain[3] > ystart+numy*pixsize)domain[3]=ystart+numy*pixsize;
        findlength(theta,rho,&domain[0],&lengthtemp[k]);
      }
    }
    rayrecvcount[p] = 0;
    for(int k = 0; k < mynumray; k++)
      if(lengthtemp[k]>0)
        rayrecvcount[p]++;
    rayrecvtemp[p] = new int[rayrecvcount[p]];
    rayrecvcount[p] = 0;
    for(int k = 0; k < mynumray; k++)
      if(lengthtemp[k]>0){
        rayrecvtemp[p][rayrecvcount[p]]=k;
        rayrecvcount[p]++;
      }
  }
  delete[] lengthtemp;
  //EXCHANGE SEND & RECV MAPS
  MPI_Alltoall(rayrecvcount,1,MPI_INTEGER,raysendcount,1,MPI_INTEGER,MPI_COMM_WORLD);
  rayrecvstart[0] = 0;
  raysendstart[0] = 0;
  for(int p = 1; p < numproc; p++){
    rayrecvstart[p] = rayrecvstart[p-1] + rayrecvcount[p-1];
    raysendstart[p] = raysendstart[p-1] + raysendcount[p-1];
  }
  raynuminc = rayrecvstart[numproc-1]+rayrecvcount[numproc-1];
  raynumout = raysendstart[numproc-1]+raysendcount[numproc-1];
  long raynumincall = raynuminc;
  long raynumoutall = raynumout;
  MPI_Allreduce(MPI_IN_PLACE,&raynumincall,1,MPI_LONG,MPI_SUM,MPI_COMM_WORLD);
  MPI_Allreduce(MPI_IN_PLACE,&raynumoutall,1,MPI_LONG,MPI_SUM,MPI_COMM_WORLD);
  printf("myid %d raynuminc %d/%li raynumout %d/%li\n",myid,raynuminc,raynumincall,raynumout,raynumoutall);
  int *raysendlist = new int[raynumout];
  rayrecvlist = new int[raynuminc];
  for(int p = 0; p < numproc; p++){
    #pragma omp parallel for
    for(int k = 0; k < rayrecvcount[p]; k++)
      rayrecvlist[rayrecvstart[p]+k] = rayrecvtemp[p][k];
    delete[] rayrecvtemp[p];
  }
  delete[] rayrecvtemp;
  MPI_Alltoallv(rayrecvlist,rayrecvcount,rayrecvstart,MPI_INTEGER,raysendlist,raysendcount,raysendstart,MPI_INTEGER,MPI_COMM_WORLD);
  //EXCHANGE RAY COORDINATES
  complex<float> *raycoorinc = new complex<float>[raynuminc];
  complex<float> *raycoorout = new complex<float>[raynumout];
  #pragma omp parallel for
  for(int k = 0; k < raynuminc; k++)
    raycoorinc[k] = raycoor[rayrecvlist[k]];
  MPI_Alltoallv(raycoorinc,rayrecvcount,rayrecvstart,MPI_COMPLEX,raycoorout,raysendcount,raysendstart,MPI_COMPLEX,MPI_COMM_WORLD);
  delete[] raycoor;
  MPI_Barrier(MPI_COMM_WORLD);
  double timep = MPI_Wtime();
  double project_time = MPI_Wtime();
  if(myid==0)printf("\nCONSTRUCT PROJECTION MATRIX\n");
  {
    int *rownz = new int[raynumout];
    #pragma omp parallel for schedule(dynamic,proj_blocksize)
    for(int k = 0; k < raynumout; k++){
      float rho = raycoorout[k].real();
      float theta = raycoorout[k].imag();
      rownz[k] = 0;
      for(int tile = spatstart[myid]; tile < spatstart[myid]+myspattile; tile++){
        float domain[4];
        domain[0]=spatll[tile].real();
        domain[1]=domain[0]+spatsize*pixsize;
        domain[2]=spatll[tile].imag();
        domain[3]=domain[2]+spatsize*pixsize;
        //REMOVE SPATIAL EDGE CONDITION
        if(domain[1] > xstart+numx*pixsize)domain[1]=xstart+numx*pixsize;
        if(domain[3] > ystart+numy*pixsize)domain[3]=ystart+numy*pixsize;
        findnumpix(theta,rho,&domain[0],&rownz[k]);
      }
    }
    int rownzmax = 0;
    for(int k = 0; k < raynumout; k++)
      if(rownz[k]>rownzmax)rownzmax=rownz[k];
    int *rowdispl = new int[raynumout+1];
    rowdispl[0] = 0;
    for(int k = 1; k < raynumout+1; k++)
      rowdispl[k] = rowdispl[k-1]+rownz[k-1];
    delete[] rownz;
    int rownztot = rowdispl[raynumout];
    long rownzall = rownztot;
    MPI_Allreduce(MPI_IN_PLACE,&rownzall,1,MPI_LONG,MPI_SUM,MPI_COMM_WORLD);
    if(myid==0)printf("CSR STORAGE: %lu (%f GB) rownzmax %d\n",rownzall,rownzall*sizeof(float)*2/1024.0/1024.0/1024.0,rownzmax);
    int *rowindex = new int[rownztot];
    #pragma omp parallel for schedule(dynamic,proj_blocksize)
    for(int k = 0; k < raynumout; k++){
      float rho = raycoorout[k].real();
      float theta = raycoorout[k].imag();
      int start = rowdispl[k];
      for(int tile = spatstart[myid]; tile < spatstart[myid]+myspattile; tile++){
        float domain[4];
        domain[0]=spatll[tile].real();
        domain[1]=domain[0]+spatsize*pixsize;
        domain[2]=spatll[tile].imag();
        domain[3]=domain[2]+spatsize*pixsize;
        //REMOVE SPATIAL EDGE CONDITION
        if(domain[1] > xstart+numx*pixsize)domain[1]=xstart+numx*pixsize;
        if(domain[3] > ystart+numy*pixsize)domain[3]=ystart+numy*pixsize;
        int offset = (tile-spatstart[myid])*spatsize*spatsize;
        int pixtemp = 0;
        findpixind(theta,rho,&domain[0],&pixtemp,offset,&rowindex[start]);
        start=start+pixtemp;
      }
    }
    proj_rownztot = rownztot;
    proj_rownzall = rownzall;
    proj_rowdispl = rowdispl;
    proj_rowindex = rowindex;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if(myid==0)printf("RAY-TRACING TIME: %e\n",MPI_Wtime()-project_time);
  project_time = MPI_Wtime();
  if(myid==0)printf("CONSTRUCT BACKPROJECTION MATRIX\n");
  {
    int *csrRowInd = new int[proj_rownztot];
    int *inter = new int[(numthreads+1)*mynumpix];;
    int *intra = new int[proj_rownztot];
    #pragma omp parallel for
    for(int k = 0; k < raynumout; k++)
      for(int n = proj_rowdispl[k]; n < proj_rowdispl[k+1]; n++)
        csrRowInd[n] = k;
    #pragma omp parallel for
    for(int n = 0; n < (numthreads+1)*mynumpix; n++)
      inter[n] = 0;
    #pragma omp parallel for
    for(int n = 0; n < proj_rownztot; n++){
      intra[n] = inter[(omp_get_thread_num()+1)*mynumpix+proj_rowindex[n]];
      inter[(omp_get_thread_num()+1)*mynumpix+proj_rowindex[n]]++;
    }
    #pragma omp parallel for
    for(int m = 0; m < mynumpix; m++)
      for(int t = 1; t < numthreads+1; t++)
        inter[t*mynumpix+m] = inter[t*mynumpix+m]+inter[(t-1)*mynumpix+m];
    int *rowdispl = new int[mynumpix+1];
    rowdispl[0] = 0;
    for(int m = 1; m < mynumpix+1; m++)
      rowdispl[m] = rowdispl[m-1] + inter[numthreads*mynumpix+m-1];
    int rownzmax = 0;
    for(int k = 0; k < mynumpix; k++){
      int rownz = rowdispl[k+1]-rowdispl[k];
      if(rownz>rownzmax)rownzmax=rownz;
    }
    int rownztot = rowdispl[mynumpix];
    long rownzall = rownztot;
    MPI_Allreduce(MPI_IN_PLACE,&rownzall,1,MPI_LONG,MPI_SUM,MPI_COMM_WORLD);
    if(myid==0)printf("CSR STORAGE: %lu (%f GB) rownzmax %d\n",rownzall,rownzall*sizeof(float)*2/1024.0/1024.0/1024.0,rownzmax);
    int *rowindex = new int[rownztot];
    #pragma omp parallel for
    for(int n = 0; n < rownztot; n++){
      rowindex[rowdispl[proj_rowindex[n]]+
      inter[omp_get_thread_num()*mynumpix+
      proj_rowindex[n]]+intra[n]] = csrRowInd[n];
    }
    delete[] inter;
    delete[] intra;
    delete[] csrRowInd;
    back_rowdispl = rowdispl;
    back_rowindex = rowindex;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if(myid==0)printf("TRANSPOSITION TIME: %e\n",MPI_Wtime()-project_time);
  project_time = MPI_Wtime();
  if(myid==0)printf("\nBLOCKING PROJECTION MATRIX\n");
  {
    int *rowindex = proj_rowindex;
    int *rowdispl = proj_rowdispl;
    int blocksize = proj_blocksize;
    int buffsize = proj_buffsize;
    int numblocks = raynumout/blocksize;
    if(raynumout%blocksize)numblocks++;
    int numblocksall = numblocks;
    MPI_Allreduce(MPI_IN_PLACE,&numblocksall,1,MPI_INTEGER,MPI_SUM,MPI_COMM_WORLD);
    if(myid==0)printf("NUMBER OF BLOCKS: %d BUFFSIZE: %d\n",numblocksall,buffsize);
    int *blocknz = new int[numblocks];
    #pragma omp parallel
    {
      int *numint = new int[mynumpix];
      #pragma omp for schedule(dynamic)
      for(int block = 0; block < numblocks; block++){
        for(int n = 0; n < mynumpix; n++)
          numint[n] = 0;
        for(int m = block*blocksize; m < (block+1)*blocksize && m < raynumout; m++)
          for(int n = rowdispl[m]; n < rowdispl[m+1]; n++)
            numint[rowindex[n]]++;
        int count = 0;
        for(int n = 0; n < mynumpix; n++)
          if(numint[n])count++;
        blocknz[block] = count/buffsize;
        if(count%buffsize)blocknz[block]++;
      }
      delete[] numint;
    }
    int *blockdispl = new int[numblocks+1];
    blockdispl[0] = 0;
    for(int block = 1; block < numblocks+1; block++)
      blockdispl[block] = blockdispl[block-1] + blocknz[block-1];
    int blocknzmax = 0;
    for(int block = 0; block < numblocks; block++)
      if(blocknz[block]>blocknzmax)blocknzmax = blocknz[block];
    delete[] blocknz;
    int blocknztot = blockdispl[numblocks];
    long blocknzall = blocknztot;
    MPI_Allreduce(MPI_IN_PLACE,&blocknzall,1,MPI_LONG,MPI_SUM,MPI_COMM_WORLD);
    if(myid==0)printf("NUMBER OF BUFFERS: %li AVERAGE BUFF/BLOCK: %f MAX BUFF/BLOCK: %d\n",blocknzall,blocknzall/(float)numblocksall,blocknzmax);
    if(myid==0)printf("BUFF MAP: %li (%f GB)\n",blocknzall*buffsize,blocknzall*buffsize/1024.0/1024.0/1024.0*sizeof(float));
    int *buffmap = new int[blocknztot*buffsize];
    int *buffnz = new int[blocknztot];
    long footprint = 0;
    #pragma omp parallel
    {
      int *numint = new int[mynumpix];
      int buffnztemp[blocknzmax];
      #pragma omp for
      for(int n = 0; n < blocknztot*buffsize; n++)
        buffmap[n] = 0;
      #pragma omp for
      for(int n = 0; n < blocknztot; n++)
        buffnz[n] = 0;
      #pragma omp for schedule(dynamic)
      for(int block = 0; block < numblocks; block++){
        for(int n = 0; n < mynumpix; n++)
          numint[n] = 0;
        for(int m = block*blocksize; m < (block+1)*blocksize && m < raynumout; m++)
          for(int n = rowdispl[m]; n < rowdispl[m+1]; n++)
            numint[rowindex[n]]++;
        int count = 0;
        for(int n = 0; n < mynumpix; n++)
          if(numint[n]){
            int buffloc = count/buffsize;
            int mapind = (blockdispl[block]+buffloc)*buffsize+count%buffsize;
            buffmap[mapind] = n;
            numint[n] = buffloc;
            count++;
          }
        #pragma omp atomic
        footprint += count;
        for(int m = block*blocksize; m < (block+1)*blocksize && m < raynumout; m++){
          for(int n = 0; n < blocknzmax; n++)
            buffnztemp[n] = 0;
          for(int n = rowdispl[m]; n < rowdispl[m+1]; n++)
            buffnztemp[numint[rowindex[n]]]++;
          for(int buff = blockdispl[block]; buff < blockdispl[block+1]; buff++){
            int buffloc = buff-blockdispl[block];
            if(buffnztemp[buffloc]>buffnz[buff])buffnz[buff]=buffnztemp[buffloc];
          }
        }
      }
      delete[] numint;
    }
    int *buffdispl = new int[blocknztot+1];
    buffdispl[0] = 0;
    for(int buff = 1; buff < blocknztot+1; buff++)
      buffdispl[buff] = buffdispl[buff-1]+buffnz[buff-1];
    int buffnzmax = 0;
    for(int buff = 0; buff < blocknztot; buff++)
      if(buffnzmax < buffnz[buff])buffnzmax = buffnz[buff];
    delete[] buffnz;
    int buffnztot = buffdispl[blocknztot];
    long buffnzall = buffnztot;
    MPI_Allreduce(MPI_IN_PLACE,&buffnzall,1,MPI_LONG,MPI_SUM,MPI_COMM_WORLD);
    MPI_Allreduce(MPI_IN_PLACE,&footprint,1,MPI_LONG,MPI_SUM,MPI_COMM_WORLD);
    if(myid==0)printf("ELLPACK STORAGE: %li (%f GB) buffnzmax: %d STORAGE EFFICIENCY: %f DATA REUSE: %f\n",buffnzall*(long)blocksize,buffnzall*(float)blocksize*sizeof(float)*1.5/1024.0/1024.0/1024.0,buffnzmax,proj_rownzall/(float)buffnzall/blocksize*1.5,proj_rownzall/(float)footprint);
    short *buffindex = new short[buffnztot*blocksize];
    #pragma omp parallel
    {
      int *numint = new int[mynumpix];
      int *numind = new int[mynumpix];
      int buffnztemp[blocknzmax];
      #pragma omp for
      for(int n = 0; n < buffnztot*blocksize; n++)
        buffindex[n] = -1;
      #pragma omp for schedule(dynamic)
      for(int block = 0; block < numblocks; block++){
        for(int n = 0; n < mynumpix; n++)
          numint[n] = 0;
        for(int m = block*blocksize; m < (block+1)*blocksize && m < raynumout; m++)
          for(int n = rowdispl[m]; n < rowdispl[m+1]; n++)
            numint[rowindex[n]]++;
        int count = 0;
        for(int n = 0; n < mynumpix; n++)
          if(numint[n]){
            int buffloc = count/buffsize;
            numint[n] = buffloc;
            numind[n] = count%buffsize;
            count++;
          }
        for(int m = block*blocksize; m < (block+1)*blocksize && m < raynumout; m++){
          for(int n = 0; n < blocknzmax; n++)
            buffnztemp[n] = 0;
          for(int n = rowdispl[m]; n < rowdispl[m+1]; n++){
            int buffloc = numint[rowindex[n]];
            int row = buffdispl[blockdispl[block]+buffloc]+buffnztemp[buffloc];
            buffindex[row*blocksize+m%blocksize] = numind[rowindex[n]];
            buffnztemp[buffloc]++;
          }
        }
      }
      delete[] numint;
      delete[] numind;
    }
    delete[] rowindex;
    proj_numblocks = numblocks;
    proj_blocknztot = blocknztot;
    proj_blocknzall = blocknzall;
    proj_blockdispl = blockdispl;
    proj_buffnztot = buffnztot;
    proj_buffnzall = buffnzall;
    proj_buffdispl = buffdispl;
    proj_buffmap = buffmap;
    proj_buffindex = buffindex;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if(myid==0)printf("BLOCKING TIME: %e\n",MPI_Wtime()-project_time);
  project_time = MPI_Wtime();
  if(myid==0)printf("BLOCKING BACKPROJECTION MATRIX\n");
  {
    int *rowindex = back_rowindex;
    int *rowdispl = back_rowdispl;
    int blocksize = back_blocksize;
    int buffsize = back_buffsize;
    int numblocks = mynumpix/blocksize;
    if(mynumpix%blocksize)numblocks++;
    int numblocksall = numblocks;
    MPI_Allreduce(MPI_IN_PLACE,&numblocksall,1,MPI_INTEGER,MPI_SUM,MPI_COMM_WORLD);
    if(myid==0)printf("NUMBER OF BLOCKS: %d BUFFSIZE: %d\n",numblocksall,buffsize);
    int *blocknz = new int[numblocks];
    #pragma omp parallel
    {
      int *numint = new int[raynumout];
      #pragma omp for schedule(dynamic)
      for(int block = 0; block < numblocks; block++){
        for(int n = 0; n < raynumout; n++)
          numint[n] = 0;
        for(int m = block*blocksize; m < (block+1)*blocksize && m < mynumpix; m++)
          for(int n = rowdispl[m]; n < rowdispl[m+1]; n++)
            numint[rowindex[n]]++;
        int count = 0;
        for(int n = 0; n < raynumout; n++)
          if(numint[n])count++;
        blocknz[block] = count/buffsize;
        if(count%buffsize)blocknz[block]++;
      }
      delete[] numint;
    }
    int *blockdispl = new int[numblocks+1];
    blockdispl[0] = 0;
    for(int block = 1; block < numblocks+1; block++)
      blockdispl[block] = blockdispl[block-1] + blocknz[block-1];
    int blocknzmax = 0;
    for(int block = 0; block < numblocks; block++)
      if(blocknz[block]>blocknzmax)blocknzmax = blocknz[block];
    delete[] blocknz;
    int blocknztot = blockdispl[numblocks];
    long blocknzall = blocknztot;
    MPI_Allreduce(MPI_IN_PLACE,&blocknzall,1,MPI_LONG,MPI_SUM,MPI_COMM_WORLD);
    if(myid==0)printf("NUMBER OF BUFFERS: %li AVERAGE BUFF/BLOCK: %f MAX BUFF/BLOCK: %d\n",blocknzall,blocknzall/(float)numblocksall,blocknzmax);
    if(myid==0)printf("BUFF MAP: %li (%f GB)\n",blocknzall*buffsize,blocknzall*buffsize/1024.0/1024.0/1024.0*sizeof(float));
    int *buffmap = new int[blocknztot*buffsize];
    int *buffnz = new int[blocknztot];
    long footprint = 0;
    #pragma omp parallel
    {
      int *numint = new int[raynumout];
      int buffnztemp[blocknzmax];
      #pragma omp for
      for(int n = 0; n < blocknztot*buffsize; n++)
        buffmap[n] = 0;
      #pragma omp for
      for(int n = 0; n < blocknztot; n++)
        buffnz[n] = 0;
      #pragma omp for schedule(dynamic)
      for(int block = 0; block < numblocks; block++){
        for(int n = 0; n < raynumout; n++)
          numint[n] = 0;
        for(int m = block*blocksize; m < (block+1)*blocksize && m < mynumpix; m++)
          for(int n = rowdispl[m]; n < rowdispl[m+1]; n++)
            numint[rowindex[n]]++;
        int count = 0;
        for(int n = 0; n < raynumout; n++)
          if(numint[n]){
            int buffloc = count/buffsize;
            int mapind = (blockdispl[block]+buffloc)*buffsize+count%buffsize;
            buffmap[mapind] = n;
            numint[n] = buffloc;
            count++;
          }
        #pragma omp atomic
        footprint += count;
        for(int m = block*blocksize; m < (block+1)*blocksize && m < mynumpix; m++){
          for(int n = 0; n < blocknzmax; n++)
            buffnztemp[n] = 0;
          for(int n = rowdispl[m]; n < rowdispl[m+1]; n++)
            buffnztemp[numint[rowindex[n]]]++;
          for(int buff = blockdispl[block]; buff < blockdispl[block+1]; buff++){
            int buffloc = buff-blockdispl[block];
            if(buffnztemp[buffloc]>buffnz[buff])buffnz[buff]=buffnztemp[buffloc];
          }
        }
      }
      delete[] numint;
    }
    int *buffdispl = new int[blocknztot+1];
    buffdispl[0] = 0;
    for(int buff = 1; buff < blocknztot+1; buff++)
      buffdispl[buff] = buffdispl[buff-1]+buffnz[buff-1];
    int buffnzmax = 0;
    for(int buff = 0; buff < blocknztot; buff++)
      if(buffnzmax < buffnz[buff])buffnzmax = buffnz[buff];
    delete[] buffnz;
    int buffnztot = buffdispl[blocknztot];
    long buffnzall = buffnztot;
    MPI_Allreduce(MPI_IN_PLACE,&buffnzall,1,MPI_INTEGER,MPI_SUM,MPI_COMM_WORLD);
    MPI_Allreduce(MPI_IN_PLACE,&footprint,1,MPI_LONG,MPI_SUM,MPI_COMM_WORLD);
    if(myid==0)printf("ELLPACK STORAGE: %li (%f GB) buffnzmax: %d STORAGE EFFICIENCY: %f DATA REUSE: %f\n",buffnzall*blocksize,buffnzall*(float)blocksize*sizeof(float)*1.5/1024.0/1024.0/1024.0,buffnzmax,proj_rownzall/(float)buffnzall/blocksize*1.5,proj_rownzall/(float)footprint);
    short *buffindex = new short[buffnztot*blocksize];
    #pragma omp parallel
    {
      int *numint = new int[raynumout];
      int *numind = new int[raynumout];
      int buffnztemp[blocknzmax];
      #pragma omp for
      for(int n = 0; n < buffnztot*blocksize; n++)
        buffindex[n] = -1;
      #pragma omp for schedule(dynamic)
      for(int block = 0; block < numblocks; block++){
        for(int n = 0; n < raynumout; n++)
          numint[n] = 0;
        for(int m = block*blocksize; m < (block+1)*blocksize && m < mynumpix; m++)
          for(int n = rowdispl[m]; n < rowdispl[m+1]; n++)
            numint[rowindex[n]]++;
        int count = 0;
        for(int n = 0; n < raynumout; n++)
          if(numint[n]){
            int buffloc = count/buffsize;
            numint[n] = buffloc;
            numind[n] = count%buffsize;
            count++;
          }
        for(int m = block*blocksize; m < (block+1)*blocksize && m < mynumpix; m++){
          for(int n = 0; n < blocknzmax; n++)
            buffnztemp[n] = 0;
          for(int n = rowdispl[m]; n < rowdispl[m+1]; n++){
            int buffloc = numint[rowindex[n]];
            int row = buffdispl[blockdispl[block]+buffloc]+buffnztemp[buffloc];
            buffindex[row*blocksize+m%blocksize] = numind[rowindex[n]];
            buffnztemp[buffloc]++;
          }
        }
      }
      delete[] numint;
      delete[] numind;
    }
    delete[] rowindex;
    back_numblocks = numblocks;
    back_blocknztot = blocknztot;
    back_blocknzall = blocknzall;
    back_blockdispl = blockdispl;
    back_buffnztot = buffnztot;
    back_buffnzall = buffnzall;
    back_buffdispl = buffdispl;
    back_buffmap = buffmap;
    back_buffindex = buffindex;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if(myid==0)printf("BLOCKING TIME: %e\n",MPI_Wtime()-project_time);
  project_time = MPI_Wtime();
  if(myid==0)printf("\nFILL PROJECTION MATRIX\n");
  {
    proj_buffvalue = new float[proj_buffnztot*proj_blocksize];
    #pragma omp parallel for
    for(int n = 0; n < proj_buffnztot*proj_blocksize; n++)
      proj_buffvalue[n] = 0;
    #pragma omp parallel for schedule(dynamic)
    for(int block = 0; block < proj_numblocks; block++)
      for(int k = block*proj_blocksize; k < (block+1)*proj_blocksize && k < raynumout; k++){
        float rho = raycoorout[k].real();
        float theta = raycoorout[k].imag();
        for(int buff = proj_blockdispl[block]; buff < proj_blockdispl[block+1]; buff++)
          for(int row = proj_buffdispl[buff]; row < proj_buffdispl[buff+1]; row++){
            int ind = row*proj_blocksize+k%proj_blocksize;
            if(proj_buffindex[ind]==-1)
              proj_buffindex[ind] = 0;
            else{
              int mapind = buff*proj_buffsize+proj_buffindex[ind];
              int pixind = proj_buffmap[mapind];
              float domain[4];
              domain[0]=pixcoor[pixind].real()-pixsize/2;
              domain[1]=domain[0]+pixsize;
              domain[2]=pixcoor[pixind].imag()-pixsize/2;
              domain[3]=domain[2]+pixsize;
              findlength(theta,rho,&domain[0],&proj_buffvalue[ind]);
            }
          }
      }
    #pragma omp parallel for
    for(int n = 0; n < proj_buffnztot*proj_blocksize; n++)
      if(proj_buffindex[n]==-1){
        proj_buffindex[n] = 0;
        proj_buffvalue[n] = 0;
      }
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if(myid==0)printf("TIME: %e\n",MPI_Wtime()-project_time);
  project_time = MPI_Wtime();
  if(myid==0)printf("FILL BACKPROJECTION MATRIX\n");
  {
    back_buffvalue = new float[back_buffnztot*back_blocksize];
    #pragma omp parallel for
    for(int n = 0; n < back_buffnztot*back_blocksize; n++)
      back_buffvalue[n] = 0;
    #pragma omp parallel for schedule(dynamic)
    for(int block = 0; block < back_numblocks; block++)
      for(int n = block*back_blocksize; n < (block+1)*back_blocksize && n < mynumpix; n++){
        float domain[4];
        domain[0]=pixcoor[n].real()-pixsize/2;
        domain[1]=domain[0]+pixsize;
        domain[2]=pixcoor[n].imag()-pixsize/2;
        domain[3]=domain[2]+pixsize;
        for(int buff = back_blockdispl[block]; buff < back_blockdispl[block+1]; buff++)
          for(int row = back_buffdispl[buff]; row < back_buffdispl[buff+1]; row++){
            int ind = row*back_blocksize+n%back_blocksize;
            if(back_buffindex[ind]==-1)
              back_buffindex[ind] = 0;
            else{
              int mapind = buff*back_buffsize+back_buffindex[ind];
              int rayind = back_buffmap[mapind];
              float rho = raycoorout[rayind].real();
              float theta = raycoorout[rayind].imag();
              findlength(theta,rho,&domain[0],&back_buffvalue[ind]);
            }
          }
      }
    #pragma omp parallel for
    for(int n = 0; n < back_buffnztot*back_blocksize; n++)
      if(back_buffindex[n]==-1){
        back_buffindex[n] = 0;
        back_buffvalue[n] = 0;
      }
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if(myid==0)printf("TIME: %e\n",MPI_Wtime()-project_time);
  delete[] raycoorout;
  delete[] raycoorinc;
  if(myid==0)printf("REDUCTION MAPPINGS\n");
  //FIND RAY-TO-RAY MAPPING
  int *raynumray = new int[mynumray];
  #pragma omp parallel for
  for(int k = 0; k < mynumray; k++)
    raynumray[k]=0;
  for(int k = 0; k < raynuminc; k++)
    raynumray[rayrecvlist[k]]++;
  rayraystart = new int[mynumray+1];
  rayraystart[0] = 0;
  for(int k = 1; k < mynumray+1; k++)
    rayraystart[k] = rayraystart[k-1] + raynumray[k-1];
  rayrayind = new int[raynuminc];
  #pragma omp parallel for
  for(int k = 0; k < mynumray; k++)raynumray[k]=0;
  for(int k = 0; k < raynuminc; k++){
    rayrayind[rayraystart[rayrecvlist[k]]+raynumray[rayrecvlist[k]]] = k;
    raynumray[rayrecvlist[k]]++;
  }
  delete[] raynumray;
  MPI_Barrier(MPI_COMM_WORLD);
  if(myid==0)printf("PREPROCESSING TIME: %e\n",MPI_Wtime()-timep);

  /*printf("projection matrix\n");
  for(int m = 0; m < raynumout; m++){
    for(int n = raypixstart[m]; n < raypixstart[m+1]; n++)
      printf("%d ",raypixind[n]);
    printf("\n");
  }
  printf("backprojection matrix\n");
  for(int m = 0; m < mynumpix; m++){
    for(int n = pixraystart[m]; n < pixraystart[m+1]; n++)
      printf("%d ",pixrayind[n]);
    printf("\n");
  }*/

  //raypart = new float[raynumout]; //PARTIAL MEASUREMENT
  //raybuff = new float[raynuminc]; //INPUT BUFFER
  float *obj;// = new float[mynumpix]; //OBJECT
  float *mes;// = new float[mynumray]; //MEASUREMENT
  float *ray;// = new float[mynumray]; //RAYSUM
  float *res;// = new float[mynumray]; //RESIDUAL ERRORKEK
  float *gra;// = new float[mynumpix]; //GRADIENT
  float *dir;// = new float[mynumpix]; //DIRECTION

  setup_gpu(&obj,&gra,&dir,&mes,&res,&ray);

  numproj = 0;
  numback = 0;

  /*if(myid==0)printf("SIMULATION STARTS\n");
  #pragma omp parallel for
  for(int n = 0; n < mynumpix; n++){
    obj[n] = 0;
    if(pixcoor[n].real() > xstart/2 && pixcoor[n].real() < 0 )
      if(pixcoor[n].imag() > ystart/2 && pixcoor[n].imag() < 0)
        obj[n] = 1;
    if(pixcoor[n].real() < -xstart/2 && pixcoor[n].real() > 0)
      if(pixcoor[n].imag() < -ystart/2 && pixcoor[n].imag() > 0)
        obj[n] = 1;
  }
  //SIMULATION
  projection(mes,obj);
  if(myid==0)printf("SIMULATION ENDS\n");*/
  if(myid==0)printf("INPUT MEASUREMENT DATA\n");
  float *mesdata = new float[numrho*numthe];
  FILE *dataf = fopen(sinfile,"rb");
  fread(mesdata,sizeof(float),numrho*numthe,dataf);
  fclose(dataf);
  #pragma omp parallel for
  for(int k = 0; k < mynumray; k++)
    if(raymesind[k]>-1)mes[k] = mesdata[raymesind[k]];
    else mes[k] = 0;
  delete[] mesdata;
  if(myid==0)printf("INPUT ENDS\n");
  /*float *mesall = new float[numray];
  #pragma omp parallel for
  for(int n = 0; n < numray; n++)
    mesall[n] = 0;
  #pragma omp parallel for
  for(int k = 0; k < mynumray; k++)
    mesall[rayglobalind[k]] = mes[k];
  MPI_Allreduce(MPI_IN_PLACE,mesall,numray,MPI_FLOAT,MPI_SUM,MPI_COMM_WORLD);
  if(myid==0){
    FILE *mesf = fopen("measurement.bin","wb");
    fwrite(mesall,sizeof(float),numray,mesf);
    fclose(mesf);
  }
  delete[] mesall;*/
  delete[] rayglobalind;
  delete[] raymesind;
  if(myid==0)printf("GRADIENT-DESCENT OPTIMIZATION\n");
  //FILE *resf = fopen("residual.txt","w");
  //INITIALIZE OBJECT
  #pragma omp parallel for
  for(int n = 0; n < mynumpix; n++)
    obj[n] = 0;
  MPI_Barrier(MPI_COMM_WORLD);
  {
    double time;
    double ctime = 0;
    double wtime = 0;
    double rtime = MPI_Wtime();
    //FORWARD PROJECTION
    projection(ray,obj);
    //FIND RESIDUAL ERROR
    time = MPI_Wtime();
    subtract_kernel(res,ray,mes,mynumray);
    MPI_Barrier(MPI_COMM_WORLD);
    ctime = ctime + MPI_Wtime() - time;
    //FIND GRADIENT
    backprojection(gra,res);
    time = MPI_Wtime();
    float error = norm_kernel(res,mynumray);
    float gradnorm = norm_kernel(gra,mynumpix);
    MPI_Barrier(MPI_COMM_WORLD);
    ctime = ctime + MPI_Wtime() - time;
    time = MPI_Wtime();
    MPI_Allreduce(MPI_IN_PLACE,&error,1,MPI_FLOAT,MPI_SUM,MPI_COMM_WORLD);
    MPI_Allreduce(MPI_IN_PLACE,&gradnorm,1,MPI_FLOAT,MPI_SUM,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    wtime = wtime + MPI_Wtime() - time;
    if(myid==0)printf("iter: %d error: %e gradnorm: %e\n",0,error,gradnorm);
    //SAVE DIRECTION
    time = MPI_Wtime();
    copy_kernel(dir,gra,mynumpix);
    float oldgradnorm = gradnorm;
    MPI_Barrier(MPI_COMM_WORLD);
    ctime = ctime + MPI_Wtime() - time;
    //START ITERATIONS
    for(int iter = 1; iter <= numiter; iter++){
      //PROJECT DIRECTION
      projection(ray,dir);
      //FIND STEP SIZE
      time = MPI_Wtime();
      float temp1 = dot_kernel(res,ray,mynumray);
      float temp2 = norm_kernel(ray,mynumray);
      MPI_Barrier(MPI_COMM_WORLD);
      ctime = ctime + MPI_Wtime() - time;
      time = MPI_Wtime();
      MPI_Allreduce(MPI_IN_PLACE,&temp1,1,MPI_FLOAT,MPI_SUM,MPI_COMM_WORLD);
      MPI_Allreduce(MPI_IN_PLACE,&temp2,1,MPI_FLOAT,MPI_SUM,MPI_COMM_WORLD);
      MPI_Barrier(MPI_COMM_WORLD);
      wtime = wtime + MPI_Wtime() - time;
      //STEP SIZE
      time = MPI_Wtime();
      float alpha = temp1/temp2;
      saxpy_kernel(obj,obj,-1.0*alpha,dir,mynumpix);
      MPI_Barrier(MPI_COMM_WORLD);
      ctime = ctime + MPI_Wtime() - time;
      //FORWARD PROJECTION
      projection(ray,obj);
      //FIND RESIDUAL ERROR
      time = MPI_Wtime();
      subtract_kernel(res,ray,mes,mynumray);
      MPI_Barrier(MPI_COMM_WORLD);
      ctime = ctime + MPI_Wtime() - time;
      //FIND GRADIENT
      backprojection(gra,res);
      time = MPI_Wtime();
      float error = norm_kernel(res,mynumray);
      float gradnorm = norm_kernel(gra,mynumpix);
      MPI_Barrier(MPI_COMM_WORLD);
      ctime = ctime + MPI_Wtime() - time;
      time = MPI_Wtime();
      MPI_Allreduce(MPI_IN_PLACE,&error,1,MPI_FLOAT,MPI_SUM,MPI_COMM_WORLD);
      MPI_Allreduce(MPI_IN_PLACE,&gradnorm,1,MPI_FLOAT,MPI_SUM,MPI_COMM_WORLD);
      MPI_Barrier(MPI_COMM_WORLD);
      wtime = wtime + MPI_Wtime() - time;
      //UPDATE DIRECTION
      time = MPI_Wtime();
      if(myid==0)printf("iter: %d error: %e gradnorm: %e\n",iter,error,gradnorm);
      //fprintf(resf,"%e %e\n",error,gradnorm);
      float beta = gradnorm/oldgradnorm;
      //float beta = 0;
      oldgradnorm = gradnorm;
      saxpy_kernel(dir,gra,beta,dir,mynumpix);
      MPI_Barrier(MPI_COMM_WORLD);
      ctime = ctime + MPI_Wtime() - time;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    rtime = MPI_Wtime()-rtime;
    //fclose(resf);

    if(myid==0)printf("recon: %e proj: %e (%e %e %e) backproj: %e (%e %e %e) ctime: %e wtime: %e\n",rtime,ftime,fktime,aftime,frtime,btime,brtime,abtime,bktime,ctime,wtime);

    if(myid==0)printf("numproj: %d numback: %d\n",numproj,numback);
    MPI_Allreduce(MPI_IN_PLACE,&fktime,1,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
    MPI_Allreduce(MPI_IN_PLACE,&bktime,1,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
    double projflops = proj_rownzall/1e9*2*numproj/fktime;
    double backflops = proj_rownzall/1e9*2*numback/bktime;
    double totflops = proj_rownzall/1e9*2*(numproj+numback)/(fktime+bktime);
    double projbw = (proj_buffnzall*proj_blocksize/1e9*6+proj_blocknzall*proj_buffsize/1e9*4)*numproj/fktime;
    double backbw = (back_buffnzall*back_blocksize/1e9*6+back_blocknzall*back_buffsize/1e9*4)*numback/bktime;
    double totbw = ((proj_buffnzall*proj_blocksize/1e9*6+proj_blocknzall*proj_buffsize/1e9*4)*numproj+(back_buffnzall*back_blocksize/1e9*6+back_blocknzall*back_buffsize/1e9*4)*numback)/(fktime+bktime);
    if(myid==0)printf("proj: %e s (%f GFLOPS) backproj: %e s (%f GFLOPS) avGFLOPS: %f totGFLOPS: %f\n",fktime,projflops,bktime,backflops,totflops,totflops*numproc);
    if(myid==0)printf("proj: %f GB/s back: %f GB/s av: %f GB/s tot: %f GB/s\n",projbw,backbw,totbw,totbw*numproc);

  }
  float *objall = new float[numpix];
  #pragma omp parallel for
  for(int n = 0; n < numpix; n++)
    objall[n] = 0;
  #pragma omp parallel for
  for(int n = 0; n < mynumpix; n++)
    objall[pixglobalind[n]] = obj[n];
  MPI_Allreduce(MPI_IN_PLACE,objall,numpix,MPI_FLOAT,MPI_SUM,MPI_COMM_WORLD);
  if(myid==0){
    FILE *objf = fopen(outfile,"wb");
    fwrite(objall,sizeof(float),numpix,objf);
    fclose(objf);
  }
  delete[] objall;

  MPI_Barrier(MPI_COMM_WORLD);
  if(myid==0)printf("Total Time: %e\n",MPI_Wtime()-timetot);

  MPI_Finalize();
}

float norm_kernel(float *a, int dim){
  float reduce = 0;
  #pragma omp parallel for reduction(+:reduce)
  for(int n = 0; n < dim; n++)
    reduce += norm(a[n]);
  return reduce;
};
float dot_kernel(float *a, float *b, int dim){
  float reduce = 0;
  #pragma omp parallel for reduction(+:reduce)
  for(int n = 0; n < dim; n++)
    reduce += a[n]*b[n];
  return reduce;
};
void saxpy_kernel(float *a, float *b, float coef, float *c, int dim){
  #pragma omp parallel for
  for(int n = 0; n < dim; n++)
    a[n] = b[n] + coef*c[n];
};
void copy_kernel(float *a, float *b, int dim){
  #pragma omp parallel for
  for(int n = 0; n < dim; n++)
    a[n] = b[n];
};
void subtract_kernel(float *a, float *b, float *c, int dim){
  #pragma omp parallel for
  for(int n = 0; n < dim; n++)
    a[n] = b[n] - c[n];
};
