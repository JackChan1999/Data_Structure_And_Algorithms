## 用LinkedList模拟栈
```java
public class MyStack {

	private LinkedList link;

	public MyStack() {
		link = new LinkedList();
	}

	public void add(Object obj) {
		link.addFirst(obj);
	}

	public Object get() {
		return link.removeFirst();
	}

	public boolean isEmpty() {
		return link.isEmpty();
	}
}
```
### 栈的数组实现
```java
class StackX
{
   private int maxSize;        
   private long[] stackArray;
   private int top;            

   public StackX(int s)         
   {
      maxSize = s;             
      stackArray = new long[maxSize];  
      top = -1;                
   }

   public void push(long j)    
   {
      stackArray[++top] = j;     
   }

   public long pop()           
   {
      return stackArray[top--];  
   }

   public long peek()          
   {
      return stackArray[top];
   }

   public boolean isEmpty()    
   {
      return (top == -1);
   }

   public boolean isFull()     
   {
      return (top == maxSize-1);
   }
}  

class StackApp
{
   public static void main(String[] args)
   {
      StackX theStack = new StackX(10);  
      theStack.push(20);               
      theStack.push(40);
      theStack.push(60);
      theStack.push(80);

      while( !theStack.isEmpty() )     
      {                             
         long value = theStack.pop();
         System.out.print(value);      
         System.out.print(" ");
      }  
      System.out.println("");
     }  
}  
```
### 栈的链表实现

```java
class Link
{
   public long dData;             
   public Link next;              

   public Link(long dd)           
   { 
     dData = dd; 
   }

   public void displayLink()      
   { 
     System.out.print(dData + " "); 
   }
}  

class LinkList
{
   private Link first;            

   public LinkList()              
   { 
     first = null; 
   }           

   public boolean isEmpty()      
   { 
     return (first==null); 
   }

   public void insertFirst(long dd)
   {                          
      Link newLink = new Link(dd);
      newLink.next = first;       
      first = newLink;            
   }

   public long deleteFirst()      
   {                           
      Link temp = first;          
      first = first.next;         
      return temp.dData;          
   }

   public void displayList()
   {
      Link current = first;       
      while(current != null)      
      {
         current.displayLink();   
         current = current.next;  
      }
      System.out.println("");
    }
}  

class LinkStack
{
   private LinkList theList;

   public LinkStack()             
   {
      theList = new LinkList();
   }

   public void push(long j)     
   {
      theList.insertFirst(j);
   }

   public long pop()            
   {
      return theList.deleteFirst();
   }

   public boolean isEmpty()       
   {
      return ( theList.isEmpty() );
   }

   public void displayStack()
   {
      System.out.print("Stack (top-->bottom): ");
      theList.displayList();
    }
} 

class LinkStackApp
{
   public static void main(String[] args)
   {
      LinkStack theStack = new LinkStack(); 

      theStack.push(20);                    
      theStack.push(40);

      theStack.displayStack();             

      theStack.push(60);                    
      theStack.push(80);

      theStack.displayStack();            

      theStack.pop();                      
      theStack.pop();

      theStack.displayStack();             
   }  
}  
```