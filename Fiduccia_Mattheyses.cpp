#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
using namespace std;


struct display_cell
{
	long int cname;
	int ap;
	int group; //which side of the partition the block is in
	struct display_cell * next;
	struct display_cell * down;
}display_cell_st;

struct free_cell
{
	long int fname;
	int ap;
	////long int area;
	int group; //which side of the partition the block is in
	int external;
	int internal;
	int gain;
	int lock;
	int lock_status;
	struct free_cell * link;
}free_cell_st;



int main()
{
	int pmax;
	int start_s=clock();
	/*
	 * ap = 0 if it is cell(a)
	 * ap = 1 if it is pad(p)
	 */

	/* Distinct elements in the file*/
	free_cell *head_f = NULL;
	head_f =(free_cell*)malloc(sizeof(free_cell));
	free_cell *ptr_f = NULL;
	ptr_f =(free_cell*)malloc(sizeof(free_cell));
	string line1,line2,line3;
	int offset;
	ptr_f=head_f;
	////long int total_area=0;
	////long int area_A=0, area_B=0;
	int counter = 0;
	ifstream myfile ("ibm01.are");


	if (myfile.is_open())
	{
		while ( getline (myfile,line1))
		{
			counter++;
			offset=line1.find(" ");
			line2=line1.substr(1,offset);
			string first_letter;
			first_letter = line1.substr(0,1);
			if(first_letter.compare("a")==0)
			{
				ptr_f->ap=0;
				ptr_f->lock=0;
			}
			if(first_letter.compare("p")==0)
			{
				ptr_f->ap=1;
				ptr_f->lock=1;
			}
			line3=line1.substr(offset+1,line1.length());
			ptr_f->fname=strtol(line2.c_str(),NULL,10);
			//ptr_f->area = strtol(line3,NULL,10);
			////long int area;
			////area = ptr_f->area;
			cout<<"node number.....**"<<ptr_f->fname<<"**";
			ptr_f->group=rand() % 2;
			cout<<"node group....**"<<ptr_f->group<<"**";
			ptr_f->internal=0;
			ptr_f->external=0;
			ptr_f->link =(free_cell*)malloc(sizeof(free_cell));
			ptr_f=ptr_f->link;
			////total_area = total_area + area;
			////cout<<"node area....**"<<area<<"\n";
			////if(ptr_f->group==0)
			////{
				////area_A=area+area_A;
			////}
			////else
			////{
				////area_B=area+area_B;
			////}
		}
		myfile.close();
	}

	/*this part is a 2-D linked list*/
	int node_count=0;
	int node_flag=0;
	string line,line_substr;
	int s_count=0;
	int s_count_prev=0;
	display_cell * head = NULL;
	display_cell * v_pointer = NULL;
	display_cell * h_pointer = NULL;

	head =(display_cell*)malloc(sizeof(display_cell));
	v_pointer =(display_cell*)malloc(sizeof(display_cell));
	h_pointer =(display_cell*)malloc(sizeof(display_cell));
	display_cell * start = NULL;
	v_pointer=head;
	h_pointer=head;

	///////bucket formation///////////////
	ifstream myfile2 ("ibm01.net");
	if (myfile2.is_open())
	{
		while (getline (myfile2,line) )
		{
			string first_letter;
			s_count_prev=s_count;
			if(line.find("s")!=std :: string :: npos)
			{
				node_flag=1;
				s_count++;
				offset=line.find("s");
				line_substr=line.substr(1,offset-1);
				first_letter = line.substr(0,1);
				v_pointer->cname=strtol(line_substr.c_str(),NULL,10);
				if(first_letter.compare("a")==0)
				{
					v_pointer->ap=0;
				}
				if(first_letter.compare("p")==0)
				{
					v_pointer->ap=1;
				}

				h_pointer=v_pointer;
				display_cell * tempv = (display_cell*)malloc(sizeof(display_cell));
				tempv->next = NULL;
				v_pointer->next = tempv;
				v_pointer->down =(display_cell*)malloc(sizeof(display_cell));
				v_pointer=v_pointer->down;

			}

			if(s_count_prev!=s_count)
			{
				node_count=0;
			}
			else if(node_flag==1)
			{
				display_cell * temph = (display_cell*)malloc(sizeof(display_cell));
				temph->next = NULL;
				h_pointer->next = temph;
				h_pointer=h_pointer->next;
				offset=line.find(" ");
				line_substr=line.substr(1,offset);
				first_letter = line.substr(0,1);
				if(first_letter.compare("a")==0)
				{
					h_pointer->ap=0;
				}
				if(first_letter.compare("p")==0)
				{
					h_pointer->ap=1;
				}
				h_pointer->cname=strtol(line_substr.c_str(),NULL,10);

				node_count++;
			}

		}
		myfile2.close();
	}

	///////////////////////////////////////////assigning group in the 2-d LL to check for the external and internal connections///////////////////

	// traverse the free_array, find each of its element in the 2-D ll, and assign its weight to it

	int count_2d = 0;
	ptr_f=head_f;
	while(ptr_f!=NULL)
	{
		v_pointer=head;
		h_pointer=head;
		while(v_pointer!=NULL)
		{
			h_pointer=v_pointer;
			while(h_pointer!=NULL)
			{
				if((ptr_f->fname==h_pointer->cname)&&(ptr_f->ap==h_pointer->ap))
				{
					count_2d++;
					h_pointer->group=ptr_f->group;
				}
				h_pointer=h_pointer->next;
			}
			v_pointer=v_pointer->down;
		}
		// within this bracket, search the whole 2d array and assign weights
		ptr_f=ptr_f->link;
	}
	////cout<<"area..........."<<total_area<<"\n";

	///////////////to update the external and internal connections//////////////
	v_pointer=head;
	h_pointer=head;
	ptr_f=head_f;

	int cut_set_size=0;
	while(v_pointer!=NULL)
	{
		h_pointer=v_pointer;
		while(h_pointer!=NULL)
		{
			ptr_f=head_f;
			while(ptr_f!=NULL)
			{
				if(((h_pointer->cname==ptr_f->fname)&&(h_pointer->ap==ptr_f->ap))||
						((v_pointer->cname==ptr_f->fname)&&(v_pointer->ap==ptr_f->ap)))
				{
					if(h_pointer->group == v_pointer->group)
					{
						ptr_f->internal++;
					}
					else if(h_pointer->group != v_pointer->group)
					{
						cut_set_size++;
						ptr_f->external++;
					}
				}
				ptr_f=ptr_f->link;
			}
			//check and update in free_cell
			h_pointer=h_pointer->next;
		}
		v_pointer=v_pointer->down;
	}

	int pmax_node;
	ptr_f=head_f;
	int total_gain=0;
	pmax=0;
	int max_gain0=0,max_node0,max_ap0;
	int max_gain1=0,max_node1,max_ap1;

	while(ptr_f!=NULL)
	{
		ptr_f->gain=ptr_f->external-ptr_f->internal;

		ptr_f=ptr_f->link;
	}


	float ratio_factor=0;
	////ratio_factor=area_A/total_area;
	int cut;
	int alternate = 0;
	int max_gain,max_node,max_ap,group_tbc,final_group;
	int i=0;
	int unlocked_cells=1;

	while(unlocked_cells>0)
	{
        v_pointer=head;
		h_pointer=head;
		ptr_f=head_f;

		while(v_pointer!=NULL) //changing the group after shifting
		{
			h_pointer=v_pointer;
			while(h_pointer!=NULL)
			{
				ptr_f = head_f;
				while(ptr_f!=NULL)			//max_gain_calculation
				{
					if((ptr_f->group==0)&&(ptr_f->lock==0)&&(ptr_f->gain>0)) // check for locked status
					{
						ptr_f->group = 1;
						ptr_f->lock=1;       // lock the cell after moving
					}
					if((ptr_f->group==1)&&(ptr_f->lock==0)&&(ptr_f->gain>0))
					{
						ptr_f->group = 0;
						ptr_f->lock=1;       // lock the cell after moving
					}
					ptr_f->external = 0;
					ptr_f->internal = 0;

					if((h_pointer->cname==ptr_f->fname)&&(h_pointer->ap==ptr_f->ap))
					{
						h_pointer->group = ptr_f->group;
					}
					ptr_f=ptr_f->link;
				}

				//check and update in free_cell
				h_pointer=h_pointer->next;
			}
			v_pointer=v_pointer->down;
		}


		int cut_set_size_after=0;
		v_pointer=head;
		h_pointer=head;
		ptr_f=head_f;

		while(v_pointer!=NULL)    //calculating internal and external after changing the node using the bucket structure
		{
			h_pointer=v_pointer;
			while(h_pointer!=NULL)
			{
				ptr_f=head_f;
				while(ptr_f!=NULL)
				{
					if(((h_pointer->cname==ptr_f->fname)&&(h_pointer->ap==ptr_f->ap))||
							((v_pointer->cname==ptr_f->fname)&&(v_pointer->ap==ptr_f->ap)))
					{
						if(h_pointer->group == v_pointer->group)
						{
							ptr_f->internal++;
						}
						else if(h_pointer->group != v_pointer->group)
						{
							cut_set_size_after++;
							ptr_f->external++;
						}
					}

					ptr_f=ptr_f->link;
				}
				//check and update in free_cell
				h_pointer=h_pointer->next;
			}
			v_pointer=v_pointer->down;
		}

		long int reduction;
		ptr_f=head_f;

		unlocked_cells=0;
		while(ptr_f!=NULL)
		{
			if((ptr_f->lock == 0)&&(ptr_f->gain>0))
			{
				unlocked_cells ++;
			}

			ptr_f=ptr_f->link;
		}

		cout<<"cut set size............after............"<<cut_set_size_after/2<<"\n";
		reduction = (cut_set_size-cut_set_size_after)/2;
		cout<<"reduction in cutset size is  ...."<< reduction<<"\n";
	}
	int stop_s=clock();
	cout<<"cut set size............before............"<<cut_set_size/2<<"\n";
	cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
	return 0;
}
