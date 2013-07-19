/* sudoku.cpp : Solve sudoku using Dancing Links
 *
 * Author: Raziman T V
 *
 * License:
 * You may use this document for whatever purpose you desire, as long as the
 * following conditions are satisfied:
 * 1) You do not use it in a way that suggests that I endorse the use.
 * 2) If you redistribute this code, I would be grateful if you credit me as
 * the author of the code. This is not a requirement.
 * 3) If this code is modified and redistributed with credit to me as the
 * original author, the code should either contain a link to the source
 * where the unaltered code can be found, or state explicitly that the code
 * has been altered significantly since.
 */

# include <cassert>
# include <cstdio>
# include <cstring>
# include <cstdlib>
# include <ctime>
# include <algorithm>
# include "../DancingLinks/DancingLinks.h"

using namespace std;

int main()
{
  //Every placement of a number in a position is a subset. Hence R=9*9*9
  //There are constraints of 4 types:
  //  A cell can have only 1 number : 9*9 constraints
  //  A row can have a number only once : 9*9 constraints
  //  A column can have a number only once : 9*9 constraints
  //  A box can have a number only once : 9*9 constraints
  //Overall, C = 9*9*4
  const int rows=9*9*9;
  const int columns=9*9*4;

  //Existing numbers in sudoku cells result in already satisfied constraints
  //  and forbidden columns
  //ar1 stores whether a particular row is non-forbidden and is later used to
  // find the ith non-forbidden row
  //ar2 stores whether a particular column is non-forbidden and is later used to
  //  find the ith non-forbidden column
  int ar1[rows],ar2[columns];
  memset(ar1,1,sizeof ar1);
  memset(ar2,1,sizeof ar2);

  //Read the initial position of the board and make rows/columns forbidden
  //  based on existing entries
  int board[9][9];
  for(int i=0;i<9;i++)
  {
    for(int j=0;j<9;j++)
    {
      scanf("%d",&board[i][j]);
      if((board[i][j]<0)||(board[i][j]>9))
      {
        printf("Invalid input board\n");
        return 1;
      }
      if(board[i][j]!=0)
      {
        //Cell is already filled
        //Find the rows and columns it blocks
        for(int k=0;k<9;k++)
        {
          //Other numbers can't end up in the same cell now
          ar1[i*81+j*9+k]=0;

          //This number can't appear in another column in the same row
          ar1[i*81+k*9+board[i][j]-1]=0;

          //This number can't appear in another row in the same column
          ar1[k*81+j*9+board[i][j]-1]=0;

          //This number can't appear in another cell in the same box
          ar1[((i/3)*3+k/3)*81+((j/3)*3+k%3)*9+board[i][j]-1]=0;
        }

        //Cell constraint satisfied
        ar2[i*9+j]=0;

        //Row constraint satisfied
        ar2[81+i*9+board[i][j]-1]=0;

        //Colum constraint satisfied
        ar2[162+j*9+board[i][j]-1]=0;

        //Box constraint satisfied
        ar2[243+(i/3)*27+(j/3)*9+board[i][j]-1]=0;
      }
    }
  }

  //Find partial sums of ar1 and ar2 to find out their position among
  // non-forbidden rows and columns. invar1 gives the inverse of ar1 - that is,
  // the ith non-forbidden row
  int invar1[rows];
  int cum1=0,cum2=0;
  for(int i=0;i<rows;i++)
  {
    if(ar1[i])
    {
      ar1[i]=cum1;
      invar1[cum1]=i;
      cum1++;
    }
    else ar1[i]=-1;
  }
  for(int i=0;i<columns;i++)
  {
    if(ar2[i])ar2[i]=cum2++;
    else ar2[i]=-1;
  }

  //The number of rows and columns we really need to deal with are just cum1 and
  //  cum2 respectively
  int R=cum1,C1=cum2,C2=0,C=C1+C2;

  //Matrix to store the constraint adjacency matrix
  char *mat;
  mat=new (nothrow)char[R*C];
  if(mat==NULL)
  {
    printf("Memory allocation error\n");
    return 1;
  }
  memset(mat,0,R*C);

  //For non-forbidden rows and columns, fill in elements in the matrix
  for(int i=0;i<9;i++)
  {
    for(int j=0;j<9;j++)
    {
      for(int k=0;k<9;k++)
      {
        int r=i*81+j*9+k;
        if(ar1[r]==-1)continue;
        int c1=i*9+j,c2=81+i*9+k,c3=162+j*9+k,c4=243+(i/3)*27+(j/3)*9+k;
        if(ar2[c1]!=-1)mat[ar1[r]*C+ar2[c1]]=1;
        if(ar2[c2]!=-1)mat[ar1[r]*C+ar2[c2]]=1;
        if(ar2[c3]!=-1)mat[ar1[r]*C+ar2[c3]]=1;
        if(ar2[c4]!=-1)mat[ar1[r]*C+ar2[c4]]=1;
      }
    }
  }

  //Now just solve it with dancing links
  //Note that we are asking it to find only one solution
  DancingLinks N;
  N.Create(R,C1,C2,mat);
  std::vector<std::vector<int> >all;
  std:: vector<int> cur;
  N.solve(all,cur,false);

  if(all.size()==1)
  {
    //Fill in the board using chosen subsets
    for(int i=0;i<all[0].size();i++)
    {
      int x=invar1[all[0][i]];
      board[x/81][(x/9)%9]=x%9+1;
    }

    for(int i=0;i<9;i++)
    {
      for(int j=0;j<9;j++)
      {
        printf("%d ",board[i][j]);
      }
      printf("\n");
    }
  }
  delete[] mat;
  return 0;
}
