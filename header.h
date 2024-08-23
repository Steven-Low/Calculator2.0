#ifndef HEADER_H
#define HEADER_H

// Header file contents


struct node* createNode(float, char, float, char*,struct node*);
struct node* createTrigNode(char*, struct node*);
void addNode(struct node*, float, char, float, char*, struct node*);
struct node* multiply(struct node*, struct node*);
struct node* divide(struct node*, struct node*);
struct node* setNegation(struct node*);
struct node* setExponent(struct node*, float);
struct node* add(struct node*, struct node*);
struct node* subtract(struct node*, struct node*);
struct node* copyNode(struct node*);
struct node* copyOneNode(struct node*);
void removeNode(struct node*, struct node*);
void removeZero(struct node*);
void mergeSort(struct node*, int, int);
void merge(struct node*, int, int, int);
void display(struct node*);
void derivative(struct node*);
void integrate(struct node*);
void solve(struct node*);
int compareInner(char *, struct node*);
int compareTri(struct node*, struct node*);
int compareAll(struct node*, struct node*);
double computeNode(struct node*, double);
void clearBuffer();


struct node {
	float coef;   // coefficient
	char var;	  // variable
	float expo;   // exponential
	char *tri;	  // trigonometric
	struct node* inner;    // pointer to term inside term (X expand)
	struct node* next; // pointer to next term
};

struct node* final;

struct node* createNode(float coef, char var, float expo, char* tri, struct node* inner){
	struct node* temp = malloc(sizeof(struct node));
	temp->coef = coef;
	temp->var = var;
	temp->expo = expo;
	temp->tri = strdup(tri);
	temp->inner = inner;
	temp->next = NULL;
	return temp;
};

void addNode(struct node* head, float coef, char var, float expo, char* tri, struct node* inner){
	struct node* newNode = createNode(coef, var, expo, tri, inner);
	struct node* currNode = head;
	while(currNode->next != NULL){
		currNode = currNode->next;
	}
	currNode->next = newNode;
};

struct node* createTrigNode(char* trigFunc, struct node* expr){
	struct node* temp = createNode(1.0, '\0', 0.0, NULL,NULL);
    temp->tri = strdup(trigFunc);
    temp->inner = expr;
    return temp;
}

struct node* setExponent(struct node* f, float n){
	// Initialize nodes with exponents value
	if(f->next == NULL){
		if(f->var == '\0')
			return createNode(pow(f->coef,n),f->var,0,NULL,NULL);
		else
			return createNode(f->coef, f->var, n,NULL,NULL);
	}
	// Recursively multiply nodes with exponent expression like (x+1)^2
	else{
		struct node* temp = createNode(0,'x',0,NULL,NULL);
		struct node* curr = f;
		while(curr != NULL){
			addNode(temp,curr->coef,curr->var,curr->expo,curr->tri,curr->inner);
			curr = curr->next;
		}
		while(n > 1 && floor(n)-n == 0){
			temp->next = multiply(temp->next,f);
			n--;
		}
		return temp->next;
	}
};

struct node* multiply(struct node* a, struct node* b){
	struct node* currNodeA = a;
	struct node* currNodeB = b;
	struct node* newNode = createNode(0,'\0',0, NULL, NULL);
		while(currNodeA != NULL){
			while(currNodeB != NULL){
				if (currNodeA->tri == NULL || currNodeB->tri == NULL){
					float coef = currNodeA->coef * currNodeB->coef;
					char var = currNodeA->expo != 0 ? currNodeA->var : currNodeB->var;
					float expo = currNodeA->expo + currNodeB->expo;
					char *tri = currNodeA->tri != NULL ? currNodeA->tri : currNodeB->tri != NULL ? currNodeB->tri : NULL;
					struct node* inner = currNodeA->inner != NULL ? currNodeA->inner : currNodeB->inner != NULL ? currNodeB->inner : NULL;
					addNode(newNode,coef,var,expo,tri,inner);
				}
				else{
				// Trigonometric function multiplication
                if (strcmp(currNodeA->tri, "sin") == 0 && strcmp(currNodeB->tri, "sin") == 0) {
                    // sin(A) * sin(B) = 0.5*cos(A+B) - 0.5*cos(A-B)
					float coef = currNodeA->coef * currNodeB->coef * 0.5; // combine all coef
                    struct node* left = add(currNodeA->inner,currNodeB->inner);
                    addNode(newNode,coef,'\0',1,"cos",left);
					struct node* right = subtract(currNodeA->inner,currNodeB->inner);
					addNode(newNode,-1 * coef,'\0',1,"cos",right);
					
                } else if (strcmp(currNodeA->tri, "cos") == 0 && strcmp(currNodeB->tri, "cos") == 0) {
                    // cos(A) * cos(B) = 0.5*cos(A+B) + 0.5*cos(A-B)
					float coef = currNodeA->coef * currNodeB->coef * 0.5; // combine all coef
                    struct node* left = add(currNodeA->inner,currNodeB->inner);
                    addNode(newNode,coef,'\0',1,"cos",left);
					struct node* right = subtract(currNodeA->inner,currNodeB->inner);
					addNode(newNode,coef,'\0',1,"cos",right);

                } else if (strcmp(currNodeA->tri, "sin") == 0 && strcmp(currNodeB->tri, "cos") == 0) {
                    // sin(A) * cos(B) = 0.5*sin(A+B) + 0.5*sin(A-B)
					float coef = currNodeA->coef * currNodeB->coef * 0.5; // combine all coef
                    struct node* left = add(currNodeA->inner,currNodeB->inner);
                    addNode(newNode,coef,'\0',1,"sin",left);
					struct node* right = subtract(currNodeA->inner,currNodeB->inner);
					addNode(newNode,coef,'\0',1,"sin",right);

                } else if (strcmp(currNodeA->tri, "cos") == 0 && strcmp(currNodeB->tri, "sin") == 0) {
                    // cos(A) * sin(B) = 0.5*sin(A+B) - 0.5*sin(A-B)
					float coef = currNodeA->coef * currNodeB->coef * 0.5; // combine all coef
                    struct node* left = add(currNodeA->inner,currNodeB->inner);
                    addNode(newNode,coef,'\0',1,"sin",left);
					struct node* right = subtract(currNodeA->inner,currNodeB->inner);
					addNode(newNode,-1 * coef,'\0',1,"sin",right);
                }
				}
				
				currNodeB = currNodeB->next;
			}
			currNodeB = b;
			currNodeA = currNodeA->next;
		}

	return newNode->next;
};

struct node* divide(struct node* a, struct node* b){
	struct node* currNodeA = a;
	struct node* currNodeB = b;
	struct node* newNode = createNode(0,'\0',0, NULL, NULL);
		while(currNodeA != NULL){
			if (currNodeB != NULL){
				float coef = currNodeA->coef / currNodeB->coef;
				char var = currNodeA->expo != 0 ? currNodeA->var : currNodeB->var;
				float expo = currNodeA->expo - currNodeB->expo;
				char *tri = currNodeA->tri;
				struct node* inner = currNodeA->inner;
				addNode(newNode,coef,var,expo,tri,inner);
			}
			currNodeA = currNodeA->next;
		}
	return newNode->next;
};

struct node *add(struct node*a, struct node* b){
	struct node* newNode = createNode(0,'\0',0,NULL,NULL);
	struct node* currA = a;
	while(currA != NULL){
		addNode(newNode, currA->coef, currA->var, currA->expo, currA->tri, currA->inner);
		currA = currA->next;
	}
	struct node* currB = b;
	while(currB != NULL){
		addNode(newNode, currB->coef, currB->var, currB->expo, currB->tri, currB->inner);
		currB = currB->next;
	}
	return newNode->next;
};

struct node *subtract(struct node*a, struct node* b){
	struct node* newNode = createNode(0,'\0',0,NULL,NULL);
	struct node* currA = a;
	while(currA != NULL){
		addNode(newNode, currA->coef, currA->var, currA->expo, currA->tri, currA->inner);
		currA = currA->next;
	}
	struct node* currB = b;
	while(currB != NULL){
		addNode(newNode, currB->coef *-1, currB->var, currB->expo, currB->tri, currB->inner);
		currB = currB->next;
	}
	return newNode->next;
};

// To display the entire linked list/ equations
void display(struct node* head){
	struct node* term = head;
	
	while(term != NULL){
		if(term->coef != 0){
			if(term->coef != 1.0 || (term->expo == 0 ))
				printf("%.2f",term->coef);
			if(term->var != '\0' && term->expo != 0){
				printf("%c",term->var);
				if(term->expo != 0){
					printf("^%.2f",term->expo);
				}
			}
			if(term->tri != NULL){
				printf("%s(",term->tri);
				display(term->inner);
				printf(")");
			}
		}
		// Output zero if it is the only 0 in each hierachy
		if(term->coef == 0 && countTerms(head) < 2 && term->next == NULL)
			printf("%.2f",term->coef);
		if(term->next != NULL && term->next->coef > 0){
			printf(" + ");
		} else if(term->next != NULL && term->next->coef < 0){
			printf(" ");
		}
		
		term = term->next;
	}

}

// To sort the linked list in descending order
void merge(struct node *head, int l, int m, int h) {
    struct node *curr = head;
    struct node *arrL = createNode(0,'x',0,NULL,NULL); // m-l+1
    struct node *arrR = createNode(0,'x',0,NULL,NULL); // h-m

	int t, a;
    for (t = 0; t < l; t++) {
        curr = curr->next;
    }
    for (a = l; a <= m; a++) {
        addNode(arrL, curr->coef, curr->var, curr->expo, curr->tri, curr->inner);
        curr = curr->next;
    }

    for (a = m + 1; a <= h; a++) {
        addNode(arrR, curr->coef, curr->var, curr->expo, curr->tri, curr->inner);
        curr = curr->next;
    }

    int i = 0, j = 0, k = l;
    curr = head;
    for (t = 0; t < l; t++) {
        curr = curr->next;
    }
    arrL = arrL->next;
    arrR = arrR->next;
    while (i < m - l + 1 && j < h - m) {
        if (arrL->expo >= arrR->expo) {
            curr->coef = arrL->coef;
            curr->var = arrL->var;
            curr->expo = arrL->expo;
			curr->tri = strdup(arrL->tri);
			curr->inner = arrL->inner;
            i++;
            arrL = arrL->next;
        } else {
            curr->coef = arrR->coef;
            curr->var = arrR->var;
            curr->expo = arrR->expo;
			curr->tri = strdup(arrR->tri);
			curr->inner = arrR->inner;
            j++;
            arrR = arrR->next;
        }
        k++;
        curr = curr->next;
    }
    while (i < m - l + 1) {
        curr->coef = arrL->coef;
        curr->var = arrL->var;
        curr->expo = arrL->expo;
		curr->tri = strdup(arrL->tri);
		curr->inner = arrL->inner;
        i++;
        arrL = arrL->next;
        k++;
        curr = curr->next;
    }
    while (j < h - m) {
        curr->coef = arrR->coef;
        curr->var = arrR->var;
        curr->expo = arrR->expo;
		curr->tri = strdup(arrR->tri);
		curr->inner = arrR->inner;
        j++;
        arrR = arrR->next;
        k++;
        curr = curr->next;
    }
};

void mergeSort(struct node *head, int l, int h) {
    if (l < h) {
        int mid = (l + h) / 2;
        mergeSort(head, l, mid);
        mergeSort(head, mid + 1, h);
        merge(head, l, mid, h);
    }
};

// Count number of terms in given hierarchy
int countTerms(struct node* term){
	struct node* curr = term;
	int num = 0;
	while(curr!=NULL){
		num++;
		curr = curr->next;
	};
	return num;
}

// Function to remove a node from a linked list
void removeNode(struct node* head, struct node* nodeToRemove) {
    struct node* currNode = head;

    while (currNode != NULL) {
        if (currNode->next == nodeToRemove) {
            currNode->next = nodeToRemove->next;
            free(nodeToRemove);
            break;
        }
        currNode = currNode->next;
    }
}

// Function to recursively remove all zeros from the entire linked list
void removeAllZero(struct node* head, struct node* curr) {
    if (curr == NULL)
        return;
    removeAllZero(head,curr->inner); // Dive in deeper node
    if (curr != NULL)
        removeAllZero(head,curr->next); // Swim to next node

	if(curr->inner == NULL || (curr->inner->coef == 0 && curr->inner->next == NULL)){
		if(curr->tri != NULL){
			curr->tri = NULL;
			curr->coef = 0;
			removeNode(head,curr);
		}	
	}
}

// Compare the trigonometric attribute 
int compareTri(struct node* a, struct node* b){
	if(a->tri == NULL && b->tri == NULL)
		return 0;
	else if(a->tri != NULL ^ b->tri != NULL)
		return 0;
	else{
		return strcmp(a->tri,b->tri)==0;
	}
}

// Buffers for nodes comparison
char buffer1[300];
char buffer2[300];

void write(char* s,const char *source){
    strcat(s,source);
};

char *str(char *s, float f){
	sprintf(s,"%f",f);
	return s;
}

int compareInner(char *buf,struct node* a){

	struct node* term = a;
	char numBuffer[10];
	char varBuffer[10];

	while(term != NULL){
		if(term->coef != 0){
			if(term->coef != 1.0 || (term->expo == 0 ))
				write(buf,str(numBuffer,term->coef));
			if(term->var != '\0' && term->expo != 0){
				varBuffer[0] = term->var;
				write(buf,varBuffer);
				if(term->expo != 0){
					write(buf,str(numBuffer,term->coef));
				}
			}
			if(term->tri != NULL){
				write(buf,term->tri); write(buf,"(");
				compareInner(buf,term->inner);
			    write(buffer1,")");
			}
		}
		term = term->next;
	}
}

void clearBuffer(){
	memset(buffer1, 0, sizeof(buffer1));
	memset(buffer2, 0, sizeof(buffer1));
}

int compareAll(struct node*a, struct node*b){
	if(compareTri(a,b)){
		compareInner(buffer1,a->inner);
		compareInner(buffer2,b->inner);
		return strcmp(buffer1,buffer2) == 0;
	}else return 0;
}

// Simplify partial terms (in the same hierachy)
void simplifyPartial(struct node *final){
	struct node* current = final;
    while (current != NULL) {
        struct node *prev = current;
        struct node *next = current->next;   
        while (next != NULL) {
            if (current->inner == NULL && next->inner == NULL && current->var == next->var && current->expo == next->expo) {
				current->coef += next->coef;
                prev->next = next->next;
				removeNode(final,next);
				next = prev->next;
            } else {
                prev = next;
                next = next->next;
            }
        }       
        current = current->next;
    }
}

// Simplify partial trigo terms (in the same hierachy)
void simplifyPartialTrigo(struct node *final){
	struct node* current = final;
    while (current != NULL) {
        struct node *prev = current;
        struct node *next = current->next;   
        while (next != NULL) {
            if (compareAll(current,next) && current->var == next->var && current->expo == next->expo) {
				current->coef += next->coef;
                prev->next = next->next;
				removeNode(final,next);
				next = prev->next;
            } else {
                prev = next;
                next = next->next;
            }
			clearBuffer();
        }       
        current = current->next;
    }
}

// Simplify all terms recursively
void simplifyAll(struct node* final) {
    if (final == NULL)
        return;
	// Mergesort the exponents in descending orders
	mergeSort(final,0,countTerms(final)-1);
    simplifyAll(final->inner);
    if (final->next != NULL)
        simplifyAll(final->next);
    simplifyPartial(final);
	simplifyPartialTrigo(final);
}

// A general function to handle simplify process
void simplify(struct node *final) {
	struct node* current = final;
	simplifyAll(current);
	removeAllZero(final,final);
};

struct node* copyNode(struct node* head){
	struct node* curr = head;
	struct node* newNode = createNode(0,'\0',0,NULL,NULL);
	while(curr!=NULL){
		addNode(newNode,curr->coef,curr->var,curr->expo,curr->tri,curr->inner);
		curr = curr->next;
	}
	return newNode->next;
}

struct node* copyOneNode(struct node* curr){
	return createNode(curr->coef,curr->var,curr->expo,curr->tri,curr->inner);
}

void redirectNode(struct node* dest, struct node* source){
	dest->coef = source->coef;
	dest->var = source->var;
	dest->expo = source->expo;
	dest->tri = source->tri;
	dest->inner = source->inner;
	dest->next = source->next;
}

void derivative(struct node *equ){
	if (equ == NULL) {
		return;
	}
	derivative(equ->next);
	if (equ->tri == NULL) {
		equ->coef = equ->coef * equ->expo;
		equ->expo--;
	} else {
		if (equ->expo == 0) {
			if (strcmp(equ->tri, "sin") == 0) {
				free(equ->tri);
				equ->tri = strdup("cos");
			} else {
				free(equ->tri);
				equ->tri = strdup("sin");
				equ->coef *= -1;
			}

			struct node *temp = equ->next;

			struct node *newNode = copyNode(equ->inner);
			if(newNode){
				derivative(newNode);
				newNode = multiply(copyOneNode(equ), newNode);	
				redirectNode(equ,newNode);
				while (newNode->next != NULL) {
					newNode = newNode->next;
				}
				newNode->next = temp;
			}
		}
	}
}

void integrate(struct node *equ){
	struct node *currNode = equ;
	while(currNode!=NULL){
		currNode->expo++;
		currNode->coef = currNode->coef/currNode->expo;
		currNode = currNode->next;
	};
	addNode(equ,1,'C',1,NULL,NULL);
};

double computeNode(struct node* inner, double n){
	double total = 0;
	struct node* curr = inner;
	while(curr != NULL){
		if(curr->tri == NULL)
			total += curr->coef * pow(n,curr->expo);
		else{
			double trigoValue = (strcmp(curr->tri,"sin")) == 0? sin(computeNode(curr->inner,n)):cos(computeNode(curr->inner,n));
			total += curr->coef * pow(n,curr->expo) * trigoValue;
		}
		curr = curr->next;
	}
	return total;
}

int isTrigo(struct node* f){
	while(f != NULL){
		if(f->tri != NULL)
			return 1;
		f=f->next;
	}
	return 0;
}

void solve(struct node *f){
	// This solving requires sorted equation
	if(f->expo == 2 && !isTrigo(f)){
		float a = 0, b = 0, c = 0;
		a = f->coef;
		if(f->next != NULL){
			if(f->next->expo == 1){
				b = f->next->coef;
				c = (f->next->next != NULL) ? f->next->next->coef : 0;
			}else
				c = f->next->coef;
		}
		float x1,x2;
		x1 = ( -1 * b + sqrt( pow(b,2) - 4 * a * c)) / (2 * a);
		x2 = ( -1 * b - sqrt( pow(b,2) - 4 * a * c)) / (2 * a);
		printf("The roots of the equation: %.2f %.2f \n",x1,x2);
		//printf("Compute node: %f\n", computeNode(f,3));
	}else{
	
		// Newton Raphson Method
		struct node* currf = f;
		struct node* df = createNode(0,'x',0,NULL,NULL); // dummy value
		while(currf != NULL){
			addNode(df,currf->coef,currf->var,currf->expo,currf->tri,currf->inner);
			currf = currf->next;
		}

		derivative(df);
		
		double x1=0, x0 = 1.111; // x0 could be any guess
		double tol = 0.000001;
		double fx, dfx;
		double tri;
		int n = 0, N = 100, solution = 0;
		
		for(n=0; n<N; n++){   
			//currf = f;
			//struct node* currdf = df;
			fx = 0; 
			dfx = 0;
			fx = computeNode(f,x0);
			dfx = computeNode(df,x0);
			
			if(dfx == 0) break;
			x1 = x0 - fx/dfx;
			if(fabs(x1-x0) < tol){  // float absolute
				solution = 1;
				break;
			}
			x0 = x1;
		}

		if(solution)
			printf("The approximate solution of given polynomial is x = %.6f\n",x1);
		else
			printf("The approximate solution after 100 iterations: x = %.6f\n",x1);
		if(isTrigo(f))
			printf("The solution may diverge as trigometry present in the equation.\n");
	}
};

#endif /* HEADER_H */