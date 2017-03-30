/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2017                              *
 *  Author: Ulrich Kremer                    *
 *  Student Version                          *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

void criticalHead(Instruction* head)
{
	int j = 0;
	int k = 0;
	int instruct = 0;
	while (head->next!=NULL)
	{
		head->instrSize = 0;
		instruct++;
		head = head->next;
	}
	int** instrSize = (int**)malloc(sizeof(int*)*instruct);
	for (j=0; j<instruct; j++)
	{
		instrSize[j] = (int*)malloc(sizeof(int)*instruct);
		for (k=0; k<instruct; k++)
		{
			instrSize[j][k] = 0;
		}
	}

	while (head!=NULL)
	{
		switch (head->opcode)
		{
			case OUTPUTAI:
				head-> instrSize = 1;
				instrSize[head->field1][head->field2]++;
				instrSize[head->field1][0]++;
				break;
			case DIV:
			case MUL:
			case SUB:
			case ADD:
				if (instrSize[head->field3][0] > 0)
				{
					head->instrSize = 1;
					instrSize[head->field3][0] --;
					instrSize[head->field1][0]++;
					instrSize[head->field2][0]++;
				}
				break;
			case STOREAI:
				if ((head->field3!=0 && instrSize[head->field2][head->field3] >0) || (instrSize[head->field2][head->field3]>1 && head->field3==0))
				{
					head->instrSize = 1;
					instrSize[head->field1][0]++;
					instrSize[head->field2][head->field3]--;
					if (instrSize[head->field2][0]==0)
					{
						instrSize[head->field2][0]++;
					}
				}
				break;
			case LOADI:
				if(instrSize[head->field2][0]>0)
				{
					head->instrSize = 1;
					instrSize[head->field2][0]--;
				}
				break;
			case LOADAI:
				if(instrSize[head->field3][0] > 0)
				{
					head->instrSize = 1;
					instrSize[head->field1][head->field2]++;
					instrSize[head->field3][0]--;
					instrSize[head->field1][0] ++;
				}
				break;
		}
		head = head->prev;
	}
	for (j=0; j<instruct; j++)
	{
		free(instrSize[j]);
	}
	free(instrSize);

}

Instruction* op(Instruction* head)
{
	Instruction* headNode = head;
	while (head!=NULL && head->instrSize!=1)
	{
		Instruction* node = head;
		head = head->next;
		headNode = head;
		if (headNode!=NULL)
				headNode->prev = NULL;
		free(node);
	}
	while(head!=NULL)
	{
		Instruction* node = NULL;
		if (head->instrSize!=1)
		{
			node = head;
			if (head->next!=NULL)
			{
				head->next->prev = head->prev;
			}
			if (head->prev!=NULL)
			{
				head->prev->next = head->next;
			}
		}
		head = head->next;
		if (node)
		{
			free(node);
		}

	}
	return headNode;
}
int main()
{
	Instruction *instrNode;
	instrNode = ReadInstructionList(stdin);
	if (!instrNode) {
		WARNING("Invalid or improper formatted instructions.\n");
		exit(EXIT_FAILURE);
	}
	criticalHead(instrNode);

	instrNode = op(instrNode);
	if (instrNode)
		PrintInstructionList(stdout, instrNode);
	while (instrNode!=NULL)
	{
		Instruction* temp = instrNode;
		instrNode = instrNode->next;
		free (temp);
	}
	return EXIT_SUCCESS;
}
