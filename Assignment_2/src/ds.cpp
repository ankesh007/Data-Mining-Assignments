#include "ds.h"
#include "dbscan.h"

bgi::rtree< value_1, bgi::rstar<16> > rtree_1;
bgi::rtree< value_2, bgi::rstar<16> > rtree_2;
bgi::rtree< value_3, bgi::rstar<16> > rtree_3;
bgi::rtree< value_4, bgi::rstar<16> > rtree_4;
bgi::rtree< value_5, bgi::rstar<16> > rtree_5;

vector<value_1> result_1;
vector<value_2> result_2;
vector<value_3> result_3;
vector<value_4> result_4;
vector<value_5> result_5;

float eps;
int inst=0;
int dim=0;
float eps_2;

vector<pair<float,int> > for_one;

void init_ds(int dimension,int instances,float epsilon)
{
	eps=epsilon;
	eps_2=eps*eps;
	dim=dimension;
	inst=instances;

	switch(dim)
	{
		case 1:
		{
			// for(int i=0;i<inst;i++)
			// {
			// 	point_1 p,q;
			// 	p.set<0>(point_collection[i][0]);
			// 	q.set<0>(point_collection[i][0]);
			// 	// for(int j=0;j<dim;j++)
			// 	// {
			// 	// 	p.set<j>(point_collection[i][j]);
			// 	// 	q.set<j>(point_collection[i][j]);
			// 	// }
			// 	box_1 b(p,q);
			// 	rtree_1.insert(mp(b,i));
			// }
			for_one.resize(inst);
			for(int i=0;i<inst;i++)
				for_one[i]=mp(point_collection[i][0],i);
			sort(for_one.begin(),for_one.end());
			break;
		}
		case 2:
		{
			for(int i=0;i<inst;i++)
			{
				point_2 p,q;
				// for(int j=0;j<dim;j++)
				// {
				// 	p.set<j>(point_collection[i][j]);
				// 	q.set<j>(point_collection[i][j]);
				// }
				p.set<0>(point_collection[i][0]);
				q.set<0>(point_collection[i][0]);
				p.set<1>(point_collection[i][1]);
				q.set<1>(point_collection[i][1]);
				box_2 b(p,q);
				rtree_2.insert(mp(b,i));
			}
			break;
		}
		case 3:
		{
			for(int i=0;i<inst;i++)
			{
				point_3 p,q;
				// for(int j=0;j<dim;j++)
				// {
				// 	p.set<j>(point_collection[i][j]);
				// 	q.set<j>(point_collection[i][j]);
				// }
				p.set<0>(point_collection[i][0]);
				q.set<0>(point_collection[i][0]);
				p.set<1>(point_collection[i][1]);
				q.set<1>(point_collection[i][1]);
				p.set<2>(point_collection[i][2]);
				q.set<2>(point_collection[i][2]);
				box_3 b(p,q);
				rtree_3.insert(mp(b,i));
			}
			break;
		}
		case 4:
		{
			for(int i=0;i<inst;i++)
			{
				point_4 p,q;
				// for(int j=0;j<dim;j++)
				// {
				// 	p.set<j>(point_collection[i][j]);
				// 	q.set<j>(point_collection[i][j]);
				// }
				p.set<0>(point_collection[i][0]);
				q.set<0>(point_collection[i][0]);
				p.set<1>(point_collection[i][1]);
				q.set<1>(point_collection[i][1]);
				p.set<2>(point_collection[i][2]);
				q.set<2>(point_collection[i][2]);
				p.set<3>(point_collection[i][3]);
				q.set<3>(point_collection[i][3]);
				box_4 b(p,q);
				rtree_4.insert(mp(b,i));
			}
			break;
		}
		case 5:
		{
			for(int i=0;i<inst;i++)
			{
				point_5 p,q;
				// for(int j=0;j<dim;j++)
				// {
				// 	p.set<j>(point_collection[i][j]);
				// 	q.set<j>(point_collection[i][j]);
				// }
				p.set<0>(point_collection[i][0]);
				q.set<0>(point_collection[i][0]);
				p.set<1>(point_collection[i][1]);
				q.set<1>(point_collection[i][1]);
				p.set<2>(point_collection[i][2]);
				q.set<2>(point_collection[i][2]);
				p.set<3>(point_collection[i][3]);
				q.set<3>(point_collection[i][3]);
				p.set<4>(point_collection[i][4]);
				q.set<4>(point_collection[i][4]);

				box_5 b(p,q);
				rtree_5.insert(mp(b,i));
			}
			break;
		}
	}
}

float getL2(int i,int j)
{
	double dist=0,t;
	for(int d=0;d<dim;d++)
	{
		t=point_collection[i][d]-point_collection[j][d] ;
		dist+=t*t;
	}
	return (dist);
}


void query_ds(int index,vector<int> &neighbour)
{
	// cout<<counter++<<endl;
	switch(dim)
	{
		case 1:
		{
			// result_1.clear();
			// point_1 p,q;
			// // for(int j=0;j<dim;j++)
			// // {
			// // 	p.set<j>(point_collection[index][j]-eps);
			// // 	q.set<j>(point_collection[index][j]+eps);
			// // }
			// p.set<0>(point_collection[index][0]-eps);
			// q.set<0>(point_collection[index][0]+eps);
			// rtree_1.query(bgi::intersects(box_1(p, q)), std::back_inserter(result_1));

			// for(auto itr:result_1)
			// {
			// 	if(getL2(itr.y,index)>eps_2)
			// 		continue;
			// 	else
			// 		neighbour.pb(itr.y);
			// }
			int idx=lower_bound(for_one.begin(),for_one.end(),mp(point_collection[index][0]-eps,0))-for_one.begin();
			for(int i=idx;i<inst;i++)
			{
				if(point_collection[i][0]>(point_collection[index][0]+eps))
					break;
				neighbour.pb(idx);
			}
			break;
		}
		case 2:
		{
			result_2.clear();
			point_2 p,q;
			// for(int j=0;j<dim;j++)
			// {
			// 	p.set<j>(point_collection[index][j]-eps);
			// 	q.set<j>(point_collection[index][j]+eps);
			// }
			p.set<0>(point_collection[index][0]-eps);
			q.set<0>(point_collection[index][0]+eps);
			p.set<1>(point_collection[index][1]-eps);
			q.set<1>(point_collection[index][1]+eps);
			rtree_2.query(bgi::intersects(box_2(p, q)), std::back_inserter(result_2));

			for(auto itr:result_2)
			{
				if(getL2(itr.y,index)>eps_2)
					continue;
				else
					neighbour.pb(itr.y);
			}
			break;
		}
		case 3:
		{
			result_3.clear();
			point_3 p,q;
			// for(int j=0;j<dim;j++)
			// {
			// 	p.set<j>(point_collection[index][j]-eps);
			// 	q.set<j>(point_collection[index][j]+eps);
			// }
			p.set<0>(point_collection[index][0]-eps);
			q.set<0>(point_collection[index][0]+eps);
			p.set<1>(point_collection[index][1]-eps);
			q.set<1>(point_collection[index][1]+eps);
			p.set<2>(point_collection[index][2]-eps);
			q.set<2>(point_collection[index][2]+eps);
			rtree_3.query(bgi::intersects(box_3(p, q)), std::back_inserter(result_3));

			for(auto itr:result_3)
			{
				if(getL2(itr.y,index)>eps_2)
					continue;
				else
					neighbour.pb(itr.y);
			}
			break;
		}
		case 4:
		{
			result_4.clear();
			point_4 p,q;
			// for(int j=0;j<dim;j++)
			// {
			// 	p.set<j>(point_collection[index][j]-eps);
			// 	q.set<j>(point_collection[index][j]+eps);
			// }
			p.set<0>(point_collection[index][0]-eps);
			q.set<0>(point_collection[index][0]+eps);
			p.set<1>(point_collection[index][1]-eps);
			q.set<1>(point_collection[index][1]+eps);
			p.set<2>(point_collection[index][2]-eps);
			q.set<2>(point_collection[index][2]+eps);
			p.set<3>(point_collection[index][3]-eps);
			q.set<3>(point_collection[index][3]+eps);

			rtree_4.query(bgi::intersects(box_4(p, q)), std::back_inserter(result_4));

			for(auto itr:result_4)
			{
				if(getL2(itr.y,index)>eps_2)
					continue;
				else
					neighbour.pb(itr.y);
			}
			break;
		}
		case 5:
		{
			result_5.clear();
			point_5 p,q;
			// for(int j=0;j<dim;j++)
			// {
			// 	p.set<j>(point_collection[index][j]-eps);
			// 	q.set<j>(point_collection[index][j]+eps);
			// }
			p.set<0>(point_collection[index][0]-eps);
			q.set<0>(point_collection[index][0]+eps);
			p.set<1>(point_collection[index][1]-eps);
			q.set<1>(point_collection[index][1]+eps);
			p.set<2>(point_collection[index][2]-eps);
			q.set<2>(point_collection[index][2]+eps);
			p.set<3>(point_collection[index][3]-eps);
			q.set<3>(point_collection[index][3]+eps);
			p.set<4>(point_collection[index][4]-eps);
			q.set<4>(point_collection[index][4]+eps);

			rtree_5.query(bgi::intersects(box_5(p, q)), std::back_inserter(result_5));

			for(auto itr:result_5)
			{
				if(getL2(itr.y,index)>eps_2)
					continue;
				else
					neighbour.pb(itr.y);
			}
			break;
		}
	}
	// cout<<neighbour.size()<<endl;
}
