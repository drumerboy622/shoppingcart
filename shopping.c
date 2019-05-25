/*************************************************************
 * Author:Matthew Solbrack
 * Date: 04/20/2019
 * Subject: HW 3. This Algorithm runs through the Shipping 
 * Spree problem and attempts to do so effiecently
 * ************************************************************/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct item
{
	int itemNumber;
	int price;
	int weight;
}item;

typedef struct family
{
	int largestComboSize;
	struct item largestComboItems[30];
	int familyMemberWeightMax;
}family;

void mergeSort(int first, int last, struct item * arr)
{
	//if n = 1, done;
	if (last <= first)
		return;

	//Recursively sort
	int mid = (first + last)/2;
	mergeSort(first, mid, arr);
	mergeSort(mid+1, last, arr);
	
	//Merge the 2 sorted lists
	int placement;
	int left = first;
	int right = mid + 1;
	struct item * temp;
	temp = malloc((last+1) * sizeof(struct item)); 

	//place numbers into array as its being merged
	for (placement = first; placement <= last; placement++)
	{
		if(left == mid +1)
		{
			temp[placement] = arr[right];
			right++;
		}
		else if(right == last + 1)
		{
			temp[placement] = arr[left];
			left++;
		}
		else if (arr[left].weight < arr[right].weight)
		{

			temp[placement] = arr[left];
			left++;
		}
		else
		{
			temp[placement] = arr[right];
			right++;
		}
	}  
	

	//Move numbers from temp array to to original array
	for (placement = first; placement <= last; placement ++)
		arr[placement] = temp[placement];

	free(temp);
}	

int findNumber(struct item * itemArray, int itemSize, int number)
{
	//find number or number just less in array
	for(int i = 0; i < itemSize; i++)
	{
		if(itemArray[i].weight >= number)
			return i-1;
	
	}
	return itemSize-1;
}  		

int sumArrayWeight(struct item * itemArray, int itemSize)
{
	//calculate total sum of weight for the array 	
	int sum=0;
	for(int i = 0; i < itemSize; i++)
		sum = sum + itemArray[i].weight; 

	return sum;
}

int sumArrayPrice(struct item * itemArray, int itemSize)
{
	//calculate total sum of price for the array
	int sum = 0;
	for(int i = 0; i < itemSize; i++)
		sum = sum + itemArray[i].price;
	return sum;	  

}


void shoppingSpree( struct item * itemArray, struct family * f, int N, int F)
{
	struct item * tempArray;
	tempArray = malloc(sizeof(struct item) * N);
	int tempArraySize = 1, counter = 0;

	//first loop is for each individual family member
	for(int b = 0; b < F; b++)
	{
		//second loop is for looking at the weights of the items that are <= weight f[b] 
		//search to hopefully shorten the array
		for(int c = 0; c < findNumber(itemArray, N, f[b].familyMemberWeightMax)+1; c++)
		{
		//initialize tempArray
			tempArray[0] = itemArray[c];
			//third loop is to analyze differnet variations of weight with item c 
			//to find the maximum Price
			for(int d = c + 1; d < N; d++)
			{
				if((sumArrayWeight(tempArray, tempArraySize) + itemArray[d].weight) > f[b].familyMemberWeightMax)
				{
					//if the current weight of the tempArray is larger than 
					//the total the family member can hold, check to see if this 
					//combination is the highest possible

						if(sumArrayPrice(f[b].largestComboItems, f[b].largestComboSize) < sumArrayPrice(tempArray, tempArraySize))
 						{
							//if it is the highest possible, fill in the data
							//into the familly member struct
							f[b].largestComboSize = tempArraySize;
							for(int e = 0 ; e < tempArraySize; e++)
							{
								struct item temp;
								f[b].largestComboItems[e] = temp;
								f[b].largestComboItems[e].itemNumber = tempArray[e].itemNumber;	
								f[b].largestComboItems[e].price = tempArray[e].price;
								f[b].largestComboItems[e].weight = tempArray[e].weight;
				
							}
						}

						while(tempArraySize > 1)
						{
							tempArraySize--;

							 if((sumArrayWeight(tempArray, tempArraySize) + itemArray[d].weight) <= f[b].familyMemberWeightMax)
				
			  				{


								if(sumArrayPrice(f[b].largestComboItems, f[b].largestComboSize) < (sumArrayPrice(tempArray, tempArraySize)+ itemArray[d].price))
		                                                {
                		                                        //if it is the highest possible, fill in the data
                                		                        //into the familly member struct
                                                		        f[b].largestComboSize = tempArraySize+1;
                                                		        for(int e = 0 ; e < tempArraySize; e++)
                                                        		{
                                                                		struct item temp;
                                                               			f[b].largestComboItems[e] = temp;
                                                                		f[b].largestComboItems[e].itemNumber = tempArray[e].itemNumber;
                                                                		f[b].largestComboItems[e].price = tempArray[e].price;
                                                                		f[b].largestComboItems[e].weight = tempArray[e].weight;
										
                                                        		}
									struct item temp;
									f[b].largestComboItems[tempArraySize] = temp;
									f[b].largestComboItems[tempArraySize].itemNumber=itemArray[d].itemNumber;
									f[b].largestComboItems[tempArraySize].price=itemArray[d].price;
									f[b].largestComboItems[tempArraySize].weight=itemArray[d].weight;
 
									
                                                		}	
							}

						}

					//if not move onto a new combination but first put
					//values back to where they were
					tempArraySize = 1;
					d = d-counter;
					counter = 0;
				}
				else 
				{
					//if weight combination is still under the total the family member 
					//can handle, add the item to the bag and move on
					tempArraySize++;
					tempArray[tempArraySize-1]=itemArray[d];
					counter++;	
				}	
			}
		}
	}	

free(tempArray);					
}

int main()
{

	int i, T, N, F, placement=0;
	struct item itemArray[100];
	struct family f[30];
	int * arr;
	arr = malloc(500 * sizeof(int));
	FILE * textFile;
	FILE * resultFile;

	//Get the file information first
	if((textFile = fopen("shopping.txt", "r")))
	{
		while((fscanf(textFile, "%d", &arr[i])) != EOF)
		{
			++i;
		}
		fclose(textFile);
	}

	resultFile = fopen("results.txt", "w");
	
	if(!resultFile)
	{
		printf("result.txt failed to Open");
		exit(-1);	
	}	

	T = arr[placement];  
	placement++;	
	for(int i=0; i < T; i++)
	{
	
		N = arr[placement];
		placement++;
		for(int k=0; k < N; k++)
		{
			struct item temp;
			itemArray[k]= temp;
			itemArray[k].itemNumber = k+1;
			itemArray[k].price= arr[placement];
			placement++;
			itemArray[k].weight= arr[placement];
			placement++;
		} 
		mergeSort(0, N-1, itemArray);

		F = arr[placement];
		placement++;	
		for(int k = 0; k < F; k++)
		{
			struct family temp;
			f[k]=temp;
			f[k].familyMemberWeightMax= arr[placement];
			placement++;
 			
		}


		shoppingSpree(itemArray, f, N, F);

		int largest=0;

                fprintf(resultFile, "Test Case: %d\n", i+1);

			
		for(int z = 0; z < F; z++)
		{
			largest = largest + sumArrayPrice(f[z].largestComboItems, f[z].largestComboSize);

		} 

		fprintf(resultFile, "Total Price %d \n", largest); 	
                fprintf(resultFile, "Family Member Items:\n");

                for(int z = 0; z < F; z++)
                {
			fprintf(resultFile, "   %d: ", z+1);
			for(int y = 0; y < f[z].largestComboSize; y++)
			{
			    fprintf(resultFile, "%d ", f[z].largestComboItems[y].itemNumber);
			}	
                	fprintf(resultFile, "\n");
		}





                fprintf(resultFile, "\n\n");


	}		


	fclose(resultFile);		
	free(arr);

	return 0;
}  

