#include <opencv2\opencv.hpp>
#include <vector>
#include <list>
#include <cmath>
#include <iostream>
#include <queue>

using namespace std;
using namespace cv;

struct coord
{
    int x,y;
};

struct coord3d
{
    int x,y,z;
};

struct prava
{
    double a,b,c;
};

coord clockwise_next(int x, int y, int x1, int y1, int width, int height)
{
    coord pix;
    if (y-y1==1)
    {
        if (x1-x==1)
        {
            pix.x = x1;
            pix.y = y;
        }
        else
        {
            pix.x = x1+1;
            pix.y = y1;
        }
    }
    else if (y-y1==-1)
    {
        if (x1-x==-1)
        {
            pix.x = x1;
            pix.y = y;
        }
        else
        {
            pix.x = x1-1;
            pix.y = y1;
        }
    }
    else
    {
        pix.x = x1;
        pix.y = y + x1 - x;
    }

    if ((pix.x<0)||(pix.x>=width)||(pix.y<0)||(pix.y>=height)) pix = clockwise_next(x,y,pix.x,pix.y,width,height);

    return pix;
}

coord counterclockwise_next(int x, int y, int x1, int y1, int width, int height)
{
    coord pix;
    if (y-y1==1)
    {
        if (x1-x==-1)
        {
            pix.x = x1;
            pix.y = y;
        }
        else
        {
            pix.x = x1-1;
            pix.y = y1;
        }
    }
    else if (y-y1==-1)
    {
        if (x1-x==1)
        {
            pix.x = x1;
            pix.y = y;
        }
        else
        {
            pix.x = x1+1;
            pix.y = y1;
        }
    }
    else
    {
        pix.x = x1;
        pix.y = y - x1 + x;
    }


    if ((pix.x<0)||(pix.x>=width)||(pix.y<0)||(pix.y>=height)) pix = counterclockwise_next(x,y,pix.x,pix.y,width,height);



    //cout <<endl <<"kounterovanjem doso na "  <<pix.x <<" "  <<pix.y;


    return pix;
}


Vec3f rotationMatrixToEulerAngles(Mat &R)
{



	float sy = sqrt(R.at<double>(0, 0) * R.at<double>(0, 0) + R.at<double>(1, 0) * R.at<double>(1, 0));

	bool singular = sy < 1e-6; // If

	float x, y, z;
	if (!singular)
	{
		x = atan2(R.at<double>(2, 1), R.at<double>(2, 2));
		y = atan2(-R.at<double>(2, 0), sy);
		z = atan2(R.at<double>(1, 0), R.at<double>(0, 0));
	}
	else
	{
		x = atan2(-R.at<double>(1, 2), R.at<double>(1, 1));
		y = atan2(-R.at<double>(2, 0), sy);
		z = 0;
	}
	return Vec3f(x, y, z);



}

void binarization(vector<vector<int> >& gimage, int width, int height, int dim, int c)
{
    int maks=0;
    int minim=255;

    int localtres;

    int temp, temp2;

    for(int x=0; x<width; x+=dim)
        {
            if (x+dim>=width) temp = width;
                    else temp = x+dim;

            for(int y=0; y< height; y+=dim)
            {

                if (y+dim>=height) temp2 = height;
                            else temp2 = y+dim;

                for(int a=x; a<temp; a++)
                    {

                        for(int b=y; b<temp2; b++)
                        {
                            if (gimage[a][b] > maks) {maks = gimage[a][b];}
                            if (gimage[a][b] < minim) {minim = gimage[a][b];}
                        }

                    }

                localtres = (maks+minim)/2-c;

                for(int a=x; a<temp; a++)
                {
                    for(int b=y; b<temp2; b++)
                    {
                        if (gimage[a][b] >= localtres) {gimage[a][b]=1;}
                        else  {gimage[a][b]=0;}
                    }
                }
            }
        }

}

void suzukiabe(vector<vector<int> >& gimage, int width, int height, int connected_case)
{
    int nbd, lnbd;

    coord pix;

    nbd=1;

    int x1, y1, x2, x3, x4, y2, y3, y4;

    bool examinedzero;


    for (int y=0; y<height; y++)
    {

        lnbd=1;

        for (int x=0; x<width; x++)
        {
            if (gimage[x][y]!=0)
            {

                //prvi deo


                if ((gimage[x][y]==1)&&(x!=0)&&(gimage[x-1][y]==0))
                {
                    nbd++;

                    x2=x-1;
                    y2=y;
                }
                else if ((gimage[x][y]>=1)&&(x<width-1)&&(gimage[x+1][y]==0))
                {
                    nbd++;

                    if (gimage[x][y]>1) lnbd = gimage[x][y];

                    x2=x+1;
                    y2=y;
                }
                else goto cetvrti;


                //odje treba drugi deo

                //treci deo

                pix.x=x2;
                pix.y=y2;

                for (int i=0; i<8; i++)
                    if (gimage[pix.x][pix.y]==0)
                    {

                        pix = clockwise_next(x,y,pix.x,pix.y,width,height);

                    }
                    else break;

                x1=pix.x;
                y1=pix.y;

                if (gimage[x1][y1]==0)
                {
                    gimage[x][y] = -1; //-nbd;
                    goto cetvrti;
                }

                x2=x1;
                y2=y1;
                x3=x;
                y3=y;

                trecitreci:



                //cout <<x2 <<" " <<y2 <<" " <<x3 <<" " <<y3 <<endl;


                examinedzero = false;

                pix.x = x2;
                pix.y = y2;

                pix = counterclockwise_next(x3,y3,pix.x,pix.y,width,height);

                for (int i=0; i<8; i++)
                    if (gimage[pix.x][pix.y]==0)
                    {


                        if ((pix.x==x3+1)&&(pix.y==y3)) examinedzero = true;


                        pix = counterclockwise_next(x3,y3,pix.x,pix.y,width,height);
                    }
                    else break;

                x4=pix.x;
                y4=pix.y;

                if (examinedzero)
                {
                    gimage[x3][y3]=-1; //-nbd;
                    //cout <<endl <<"da sam centralnom minus nbd";
                }
                else if (gimage[x3][y3]==1)
                    {
                        gimage[x3][y3]=-1; //nbd;
                        //cout <<endl <<"da sam centralno nbd";
                    }


                if (!((x4==x)&&(y4==y)&&(x3==x1)&&(y3==y1)))
                {
                    x2=x3;
                    y2=y3;
                    x3=x4;
                    y3=y4;

                    //cout <<endl <<"centralni " <<x3 <<" " <<y3 <<"   krecem kounter od " <<x2 <<" " <<y2;


                    goto trecitreci;
                }

                //cetvrti deo

                cetvrti:


                if (gimage[x][y]!=0) lnbd=abs(gimage[x][y]);
            }

            /*
            cout <<endl;
            for (int i=0;i<8;i++)
            {

                cout <<endl;
                for (int j=0; j<13; j++)  cout <<gimage[j][i] <<" ";
            }
            */


        }
    }
}

queue<pair<coord,coord> > red;
void bfs(int marker, list<coord>& dots, int& dotsnumber, vector<vector<int> >& gimage, int width, int height)
{
	int x, y, x1, y1;
	int neighnumb;
	coord pixel;
	pair<coord, coord> aaaa;

	while (!red.empty())
	{
		x = red.front().first.x;
		y = red.front().first.y;

		x1 = red.front().second.x;
		y1 = red.front().second.y;

		red.pop();

		gimage[x][y] = marker;

		neighnumb = 8;

		if ((x == 0) && (x == width - 1)) neighnumb -= 3;
		if ((y == 0) && (y == height - 1)) neighnumb -= 3;
		if (neighnumb == 2) neighnumb = 3;


		pixel.x = x1;
		pixel.y = y1;



		for (int i = 0; i < neighnumb; i++)
			if (gimage[pixel.x][pixel.y] != -1)
			{
				pixel = clockwise_next(x, y, pixel.x, pixel.y, width, height);
			}
			else
			{
				dotsnumber++;
				dots.push_back(pixel);

				aaaa.first.x = pixel.x;
				aaaa.first.y = pixel.y;

				aaaa.second.x = x;
				aaaa.second.y = y;


				red.push(aaaa);
				break;
				//dfs(marker, dots, dotsnumber, pixel.x, pixel.y, x, y, gimage, width, height);

			}
	}
	

    /*
    if ((dots[dotsnumber-1].x>0)&&(gimage[dots[dotsnumber-1].x-1][dots[dotsnumber-1].y]==-1))
    {
        dotsnumber++;
        dots[dotsnumber-1].x = dots[dotsnumber-2].x-1;
        dots[dotsnumber-1].y = dots[dotsnumber-2].y;


        dfs(marker, dots, dotsnumber, gimage, width, height);
    }

    if ((dots[dotsnumber-1].x<width-1)&&(gimage[dots[dotsnumber-1].x+1][dots[dotsnumber-1].y]==-1))
    {
        dotsnumber++;
        dots[dotsnumber-1].x = dots[dotsnumber-2].x+1;
        dots[dotsnumber-1].y = dots[dotsnumber-2].y;



        dfs(marker, dots, dotsnumber, gimage, width, height);
    }

    if ((dots[dotsnumber-1].y>0)&&(gimage[dots[dotsnumber-1].y][dots[dotsnumber-1].y-1]==-1))
    {
        dotsnumber++;
        dots[dotsnumber-1].x = dots[dotsnumber-2].x;
        dots[dotsnumber-1].y = dots[dotsnumber-2].y-1;



        dfs(marker, dots, dotsnumber, gimage, width, height);
    }

    if ((dots[dotsnumber-1].y<height-1)&&(gimage[dots[dotsnumber-1].x][dots[dotsnumber-1].y+1]==-1))
    {
        dotsnumber++;
        dots[dotsnumber-1].x = dots[dotsnumber-2].x;
        dots[dotsnumber-1].y = dots[dotsnumber-2].y+1;



        dfs(marker, dots, dotsnumber, gimage, width, height);
    }

    if ((dots[dotsnumber-1].x>0)&&(dots[dotsnumber-1].y>0)&&(gimage[dots[dotsnumber-1].x-1][dots[dotsnumber-1].y-1]==-1))
    {
        dotsnumber++;
        dots[dotsnumber-1].x = dots[dotsnumber-2].x-1;
        dots[dotsnumber-1].y = dots[dotsnumber-2].y-1;


        dfs(marker, dots, dotsnumber, gimage, width, height);
    }

    if ((dots[dotsnumber-1].x>0)&&(dots[dotsnumber-1].y<height-1)&&(gimage[dots[dotsnumber-1].x-1][dots[dotsnumber-1].y+1]==-1))
    {
        dotsnumber++;
        dots[dotsnumber-1].x = dots[dotsnumber-2].x-1;
        dots[dotsnumber-1].y = dots[dotsnumber-2].y+1;


        dfs(marker, dots, dotsnumber, gimage, width, height);
    }

     if ((dots[dotsnumber-1].x<width-1)&&(dots[dotsnumber-1].y<height-1)&&(gimage[dots[dotsnumber-1].x+1][dots[dotsnumber-1].y+1]==-1))
    {
        dotsnumber++;
        dots[dotsnumber-1].x = dots[dotsnumber-2].x+1;
        dots[dotsnumber-1].y = dots[dotsnumber-2].y+1;


        dfs(marker, dots, dotsnumber, gimage, width, height);
    }


     if ((dots[dotsnumber-1].x<width-1)&&(dots[dotsnumber-1].y>0)&&(gimage[dots[dotsnumber-1].x+1][dots[dotsnumber-1].y-1]==-1))
    {
        dotsnumber++;
        dots[dotsnumber-1].x = dots[dotsnumber-2].x+1;
        dots[dotsnumber-1].y = dots[dotsnumber-2].y-1;


        dfs(marker, dots, dotsnumber, gimage, width, height);
    }
    */
}


list<coord> pravidaglas(list<coord>& dots, list<coord>::iterator l, list<coord>::iterator d, double eps)
{
    /*
    int j;
    for (j=0; l!=dots.begin(); j++) l--;
    cout <<endl <<j;
    for (int i=0; i<j; i++) l++;

    for (j=0; d!=dots.begin(); j++) d--;
    cout <<" " <<j;
    for (int i=0; i<j; i++) d++;
    */

    double a,b,c,r;
    int x,y;


    double maxm;
    maxm=0;
    list<coord>::iterator maxi;

    b=1;
    a=-(double)((*l).y-(*d).y)/((*l).x-(*d).x);
    c=-(*l).y-a*(*l).x;

    list<coord>::iterator i;
    for (i=l; i!=d; i++)
    {
        x=(*i).x;
        y=(*i).y;

        r=abs((a*x+b*y+c)/(sqrt(a*a+b*b)));

        if (maxm<r)
        {
            maxm=r;
            maxi=i;
        }
    }

    if (maxm<eps)
    {
        list<coord> lista;
        coord koord;

        koord.x = (*l).x;
        koord.y = (*l).y;
        lista.push_back(koord);

        koord.x = (*d).x;
        koord.y = (*d).y;
        lista.push_back(koord);

        return lista;
    }
    else
    {
        list<coord> lista1;
        list<coord> lista2;


        lista1 = pravidaglas(dots, l, maxi, eps);
        lista2 = pravidaglas(dots, maxi, d, eps);


        list<coord>::iterator it;
        it = lista2.begin();
        lista1.insert(lista1.end(), ++it, lista2.end());

        return lista1;
    }
}

vector<list<coord> > douglaspeucker(vector<vector<int> >& gimage, int width, int height, double eps)
{
    vector<list<coord> > rects;

    list<coord> dots,dotsdaglas;
    list<coord>::iterator it;
    int dotsnumber;


    coord poc;

    int marker = 1; 
	int x, y, y0 = -1, x0 = 0, sum = -1;
	do
	{
		sum++;

		if (sum > height - 1) x0++;
		else y0++;

		//cout << x0 << " " << y0 << endl;
		
		for (y = y0, x = x0; ((y >= 0) && (x < width)); y--, x++)
		{
			//cout << x << " " << y << endl;

			if (gimage[x][y] == -1)
			{
				marker++;


				poc.x = x;
				poc.y = y;
				dots.push_back(poc);
				dotsnumber = 1;

				poc.x--;
				poc.y--;
				poc = clockwise_next(x, y, poc.x, poc.y, width, height);


				pair<coord, coord> aaaa;
				aaaa.first.x = x;
				aaaa.first.y = y;

				aaaa.second.x = poc.x;
				aaaa.second.y = poc.y;

				red.push(aaaa);


				bfs(marker, dots, dotsnumber, gimage, width, height);


				/*
				dots.erase(dots.begin(),dots.end());
				poc.x = 3;
				poc.y = 3;
				dots.push_back(poc);
				poc.x = 7;
				poc.y = 7;
				dots.push_back(poc);
				poc.x = 10;
				poc.y = 6;
				dots.push_back(poc);
				poc.x = 13;
				poc.y = 5;
				dots.push_back(poc);
				poc.x = 15;
				poc.y = 8;
				dots.push_back(poc);
				poc.x = 16;
				poc.y = 2;
				dots.push_back(poc);
				poc.x = 19;
				poc.y = 4;
				dots.push_back(poc);
				poc.x = 21;
				poc.y = 7;
				dots.push_back(poc);
				*/


				if (dotsnumber>5)
				{

					poc = dots.front();
					dots.pop_front();
					dots.push_back(dots.front());
					dots.push_front(poc);

					it = dots.begin();
					for (int k = 0; k<dotsnumber; k++) it++;

					/*
					cout <<endl <<(*dots.begin()).x <<" " <<(*dots.begin()).y;
					cout <<endl <<(*it).x <<" " <<(*it).y;
					cout <<endl;
					*/

					dots = pravidaglas(dots, dots.begin(), it, eps);

				}

				int siz = dots.size();
				double rastmin, rast;
				int index;

				if (siz == 5)
				{
					dots.pop_back();

					/*
					rastmin = abs(-dots.front().y - dots.front().x);
					index = 0;

					for (int i = 0; i<4; i++)
					{
						rast = abs(-dots.front().y - dots.front().x);
						if (rast<rastmin)
						{
							rastmin = rast;
							index = i;
						}

						poc = dots.front();
						dots.pop_front();
						dots.push_back(poc);
					}



					for (int i = 0; i<index; i++)
					{
						poc = dots.front();
						dots.pop_front();
						dots.push_back(poc);
					}


					*/
					//cout <<endl;


					rects.push_back(dots);

					for (int i = 0; i<4; i++)
					{
						poc = dots.front();
						dots.pop_front();

						//cout <<endl <<poc.x <<" " <<poc.y;


						gimage[poc.x][poc.y] = -2;

					}

					//cout << endl;

					
				}


				dots.erase(dots.begin(), dots.end());

			}
		}

	} while (!((x0 == width - 1) && (y0 == height - 1)));

    return rects;
}

vector<vector<int> > bacimrezu(vector<vector<int> >& gimage, int minx, int miny, int maxx, int maxy, list<coord> rect)
{
    /*
    //obelezi invalidne piksele

    for (x=0; x<patch.size(); x++)
    {
        for (y=0; y<patch[x].size(); y++);
        {
            if ((x==0)&&(y==0)) continue;

            k=(double)((patch[x][y]).y)/((patch[x][y]).x);
            counter=0;

            for (int i=0; i<4; i++)
            {
                if (rect.front().x-rect.back().x==0)
                {
                    if (rect.front().x<x)
                    {
                         if (rect.front().y<rect.back().y)
                            {
                                if ((rect.front().y<k*rect.front().x)&&(rect.back().y>k*rect.back().x)) counter++;
                            }
                        else
                            {
                                if ((rect.front().y>k*rect.front().x)&&(rect.back().y<k*rect.back().x)) counter++;
                            }
                    }

                    koord = rect.front();
                    rect.pop_front();
                    rect.push_back(koord);
                    continue;
                }

                k1=(double)(rect.front().y-rect.back().y)/(rect.front().x-rect.back().x);
                n1=rect.front().y-k1*rect.front().x;

                if (k==k1)
                {
                    if (n1==0)
                    {
                        if ((rect.front().y<y)&&(rect.front().x<x)&&(rect.back().y<y)&&(rect.back().x<x))
                        {
                            patch[x][y].x = patch[x][y].y = -1;
                        }
                        else if ((rect.front().y>y)&&(rect.front().x>x)&&(rect.back().y>y)&&(rect.back().x>x))
                        {
                            patch[x][y].x = patch[x][y].y = -1;
                        }

                    }

                    koord = rect.front();
                    rect.pop_front();
                    rect.push_back(koord);
                    continue;

                }
                else
                {
                    x1=n1/(k-k1);
                    y1=x1*k;

                    if ((x1<x)&&(y1<y))
                    {
                        if (rect.front().y<rect.back().y)
                        {
                            if ((y>rect.front().y)&&(y<rect.back().y))
                            {
                                if (rect.front().x<rect.back().x)
                                {
                                    if ((x>rect.front().x)&&(x<rect.back().x)) counter++;
                                }
                                else
                                {
                                    if ((x<rect.front().x)&&(x>rect.back().x)) counter++;
                                }
                            }
                        }
                        else
                        {
                            if ((y<rect.front().y)&&(y>rect.back().y))
                            {
                                if (rect.front().x<rect.back().x)
                                {
                                    if ((x>rect.front().x)&&(x<rect.back().x)) counter++;
                                }
                                else
                                {
                                    if ((x<rect.front().x)&&(x>rect.back().x)) counter++;
                                }
                            }

                        }
                    }

                }

                koord = rect.front();
                rect.pop_front();
                rect.push_back(koord);
            }

            if (counter%2==0) patch[x][y].x = patch[x][y].y = -1;

        }
    }
    */

    vector<vector<int> > mreza(8, vector<int> (8));
    vector<vector<pair<int,int> > > kecevinule(8, vector<pair<int,int> > (8));
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
        {
            kecevinule[i][j].first=0;
            kecevinule[i][j].second=0;
        }

    coord koord;
    prava linija;
    double a,b,c;


    vector<coord> gornja(9), donja(9), leva(9), desna(9);
    vector<prava> vertikalne(9), horizontalne(9);


    //leva
    if (rect.front().x==rect.back().x)
    {
        b=0;
        a=1;
        c=-rect.front().x;
    }
    else
    {
        b=1;
        a=-(double)(rect.front().y-rect.back().y)/(rect.front().x-rect.back().x);
        c=-a*rect.front().x-rect.front().y;
    }

    linija.a=a;
    linija.b=b;
    linija.c=c;


    vertikalne[0]=linija;

    leva[0].x=rect.front().x;
    leva[0].y=rect.front().y;
    leva[8].x=rect.back().x;
    leva[8].y=rect.back().y;


    for (int i=1; i<8; i++)
    {
        leva[i].x=leva[0].x+i*((double)(leva[8].x-leva[0].x))/8;
    }
    for (int i=1; i<8; i++)
    {
        leva[i].y=leva[0].y+i*((double)(leva[8].y-leva[0].y))/8;
    }


    koord = rect.front();
    rect.pop_front();
    rect.push_back(koord);

    //gornja
    if (rect.front().x==rect.back().x)
    {
        b=0;
        a=1;
        c=-rect.front().x;
    }
    else
    {
        b=1;
        a=-(double)(rect.front().y-rect.back().y)/(rect.front().x-rect.back().x);
        c=-a*rect.front().x-rect.front().y;
    }

    linija.a=a;
    linija.b=b;
    linija.c=c;


    horizontalne[0]=linija;

    gornja[0].x=rect.back().x;
    gornja[0].y=rect.back().y;
    gornja[8].x=rect.front().x;
    gornja[8].y=rect.front().y;
    for (int i=1; i<8; i++)
    {
        gornja[i].x=gornja[0].x+i*((double)(gornja[8].x-gornja[0].x))/8;
    }
    for (int i=1; i<8; i++)
    {
        gornja[i].y=gornja[0].y+i*((double)(gornja[8].y-gornja[0].y))/8;
    }

    koord = rect.front();
    rect.pop_front();
    rect.push_back(koord);


    //desna
    if (rect.front().x==rect.back().x)
    {
        b=0;
        a=1;
        c=-rect.front().x;
    }
    else
    {
        b=1;
        a=-(double)(rect.front().y-rect.back().y)/(rect.front().x-rect.back().x);
        c=-a*rect.front().x-rect.front().y;
    }

    linija.a=a;
    linija.b=b;
    linija.c=c;

    vertikalne[8]=linija;

    desna[0].x=rect.back().x;
    desna[0].y=rect.back().y;
    desna[8].x=rect.front().x;
    desna[8].y=rect.front().y;
    for (int i=1; i<8; i++)
    {
        desna[i].x=desna[0].x+i*((double)(desna[8].x-desna[0].x))/8;
    }
    for (int i=1; i<8; i++)
    {
        desna[i].y=desna[0].y+i*((double)(desna[8].y-desna[0].y))/8;
    }

    koord = rect.front();
    rect.pop_front();
    rect.push_back(koord);


    //donja
    if (rect.front().x==rect.back().x)
    {
        b=0;
        a=1;
        c=-rect.front().x;
    }
    else
    {
        b=1;
        a=-(double)(rect.front().y-rect.back().y)/(rect.front().x-rect.back().x);
        c=-a*rect.front().x-rect.front().y;
    }

    linija.a=a;
    linija.b=b;
    linija.c=c;

    horizontalne[8]=linija;


    donja[0].x=rect.front().x;
    donja[0].y=rect.front().y;
    donja[8].x=rect.back().x;
    donja[8].y=rect.back().y;
    for (int i=1; i<8; i++)
    {
        donja[i].x=donja[0].x+i*((double)(donja[8].x-donja[0].x))/8;
    }
    for (int i=1; i<8; i++)
    {
        donja[i].y=donja[0].y+i*((double)(donja[8].y-donja[0].y))/8;
    }

    koord = rect.front();
    rect.pop_front();
    rect.push_back(koord);




    //horizontalne

    for (int i=1; i<8; i++)
    {
        if (leva[i].x==desna[i].x)
        {
            b=0;
            a=1;
            c=-leva[i].x;
        }
        else
        {
            b=1;
            a=-(double)(leva[i].y-desna[i].y)/(leva[i].x-desna[i].x);
            c=-a*leva[i].x-leva[i].y;
        }

        linija.a=a;
        linija.b=b;
        linija.c=c;


        horizontalne[i]=linija;
    }

    //vertikalne

    for (int i=1; i<8; i++)
    {
        if (gornja[i].x==donja[i].x)
        {
            b=0;
            a=1;
            c=-gornja[i].x;
        }
        else
        {
            b=1;
            a=-(double)(gornja[i].y-donja[i].y)/(gornja[i].x-donja[i].x);
            c=-a*gornja[i].x-gornja[i].y;
        }

        linija.a=a;
        linija.b=b;
        linija.c=c;


        vertikalne[i]=linija;
    }


    //proveravamo dje je svaki piksel
    bool validan=false;

    double x,y;

    for (int i=minx; i<=maxx; i++)
        for (int j=miny; j<=maxy; j++)
    {

        //izmedju kojih horizontalnih

        y=(-horizontalne[0].a*i-horizontalne[0].c)/horizontalne[0].b;
        if (y<j)
        for (int k=1; k<9; k++)
        {

            y=(-horizontalne[k].a*i-horizontalne[k].c)/horizontalne[k].b;

            if (y>j)
            {
                koord.y=k-1;
                validan = true;
                break;
            }
        }

        if (!validan)
        {
            continue;
        }
        else validan=false;


        //izmedju kojih vertikalnih

        x=(-vertikalne[0].b*j-vertikalne[0].c)/vertikalne[0].a;
        if (x<i)
        for (int k=1; k<9; k++)
        {

            x=(-vertikalne[k].b*j-vertikalne[k].c)/vertikalne[k].a;

            if (x>i)
            {
                koord.x=k-1;
                validan = true;
                break;
            }
        }

        if (validan)
        {
            if (gimage[i][j]==1) kecevinule[koord.x][koord.y].second++;
            else kecevinule[koord.x][koord.y].first++;
        }


    }

    for (int i=0; i<8; i++)
	{ 
        for (int j=0; j<8; j++)
        {
            if (kecevinule[i][j].first>kecevinule[i][j].second) mreza[i][j]=0;
            else mreza[i][j]=1;

			//cout << mreza[i][j] << " ";
        }
		//cout << endl;
	}


    return mreza;
}
/*
vector<pair<double, double> > resi_sistem(double a[], double b[])
{
    double a1,b1,b2,c1,c2;

    a1=b[1]-b[0]*a[1]/a[0];

    b1=b[2]-b[0]*a[2]/a[0];
    b2=b[4]-b[0]*a[4]/a[0];

    c1=b[3]-b[0]*a[3]/a[0];
    c2=b[5]-b[0]*a[5]/a[0];


    double d1,d2,d3;

    d1=b1*b1;
    d2=2*b1*b2-4*a1*c1;
    d3=b2*b2-4*a1*c2;

    double aprim,bprim,cprim,f1,f2;

    aprim=a[0]+a[1]*b1*b1/(4*a1*a1)+a[1]*d1/(4*a1*a1)-a[2]*b1/(2*a1);
    bprim=a[1]*2*b1*b2/(4*a1*a1)+d2*a[1]/(4*a1*a1)-a[2]*b2/(2*a1)+a[3]-a[4]*b1/(2*a1);
    cprim=a[1]*b2*b1/(4*a1*a1)+a[1]*d3/(4*a1*a1)-a[4]*b2/(2*a1)+a[5];

    f1=-a[1]*b1/(2*a1*a1)+a[2]/(2*a1);
    f2=a[4]/(2*a1);

    double q[5],r[5],s[5];

    q[0]=aprim*aprim;
    q[1]=2*aprim*bprim;
    q[2]=bprim*bprim+2*aprim*cprim;
    q[3]=2*bprim*cprim;
    q[4]=cprim*cprim;

    r[0]=d1*f1*f1;
    r[1]=d2*f1*f1+2*f1*f2*d1;
    r[2]=d2*2*f1*f2+d3*f1*f1+f2*f2*d1;
    r[3]=d2*f2*f2+2*f1*f2*d3+d3*f2*f2;
    r[4]=d3*f2*f2;

    for (int i=0; i<5; i++) s[i]=q[i]-r[i];

    double x1,y1,x2,y2,x3,x4,y3,y4;

    double p1,p2,p3,p4,p5,p6;

    p1=2*s[2]*s[2]*s[2]-9*s[1]*s[2]*s[3]+27*s[0]*s[3]*s[3]+27*s[1]*s[1]*s[4]-72*s[0]*s[2]*s[4];
    p2=p1+sqrt(-4*pow(s[2]*s[2]-3*s[1]*s[3]+12*s[0]*s[4],3)+p1*p1);
    p3=(s[2]*s[2]-3*s[1]*s[3]+12*s[0]*s[4])/(3*s[0]*pow(p2,(double)1/3))+pow(p2,(double)1/3)/(3*s[0]);
    p4=sqrt(s[1]*s[1]/(4*s[0]*s[0])-2*s[2]/(3*s[0])+p3);
    p5=s[1]*s[1]/(2*s[0]*s[0])-4*s[2]/(3*s[0])-p3;
    p6=(-s[1]*s[1]*s[1]/(s[0]*s[0]*s[0])+4*s[1]*s[2]/(s[0]*s[0])-8*s[3]/s[0])/(4*p4);

    vector<pair<double, double> > xy(4);

    xy[0].first=-s[1]/(4*s[0])-p4/2-sqrt(p5-p6)/2;
    xy[0].second=1;

    cout<<xy[0].first <<endl;

    xy[1].first=-s[1]/(4*s[0])-p4/2+sqrt(p5-p6)/2;
    xy[1].second=1;

    cout<<xy[1].first <<endl;

    xy[2].first=-s[1]/(4*s[0])+p4/2-sqrt(p5-p6)/2;
    xy[2].second=1;

    cout<<xy[2].first <<endl;

    xy[3].first=-s[1]/(4*s[0])+p4/2+sqrt(p5-p6)/2;
    xy[3].second=1;

    cout<<xy[3].first <<endl;

    return xy;


}

vector<vector<double> > uradi_matu(double aa, double bb, double cosuv, double cosuw, double cosvw, double ab, double bc, double ac)
{
    vector<vector<double> > dist(4, vector<double> (3));

    double a[6];
    double b[6];

    a[0]=1-aa;
    a[1]=-aa;
    a[2]=2*aa*cosuv;
    a[3]=-cosvw;
    a[4]=0;
    a[5]=1;

    b[0]=-bb;
    b[1]=1-bb;
    b[2]=2*bb*cosuv;
    b[3]=0;
    b[4]=-cosuw;
    b[5]=1;

    vector<pair<double, double> > xy;

    xy = resi_sistem(a,b);

    for (int i=0; i<4; i++)
    {
        dist[i][2] = ab/sqrt(xy[i].second*xy[i].second+xy[i].first*xy[i].first-2*xy[i].second*xy[i].first*cosuv);
        dist[i][1] = dist[i][2]*xy[i].first;
        dist[i][0] = dist[i][2]*xy[i].second;

        cout <<dist[i][0] <<" " <<dist[i][1] <<" " <<dist[i][2] <<endl;
    }

    return dist;
}
*/


