## 队列的数组实现

```java
class Queue
   {
   private int maxSize;
   private long[] queArray;
   private int front;
   private int rear;
   private int nItems;

   public Queue(int s)          
      {
      maxSize = s;
      queArray = new long[maxSize];
      front = 0;
      rear = -1;
      nItems = 0;
      }

   public void insert(long j)   
      {
      if(rear == maxSize-1)         
         rear = -1;
      queArray[++rear] = j;         
      nItems++;                     
      }

   public long remove()         
      {
      long temp = queArray[front++]; 
      if(front == maxSize)           
         front = 0;
      nItems--;                      
      return temp;
      }

   public long peekFront()      
      {
      return queArray[front];
      }

   public boolean isEmpty()    
      {
      return (nItems==0);
      }

   public boolean isFull()     
      {
      return (nItems==maxSize);
      }

   public int size()           
      {
      return nItems;
      }

   }  

class QueueApp
   {
   public static void main(String[] args)
      {
      Queue theQueue = new Queue(5);  
      theQueue.insert(10);            
      theQueue.insert(20);
      theQueue.insert(30);
      theQueue.insert(40);

      theQueue.remove();              
      theQueue.remove();
      theQueue.remove();

      theQueue.insert(50);            
      theQueue.insert(60);
      theQueue.insert(70);
      theQueue.insert(80);

      while( !theQueue.isEmpty() )    
         {                            
         long n = theQueue.remove();
         System.out.print(n);
         System.out.print(" ");
         }
      System.out.println("");
      }  
   }  
```

## 队列的链表实现

```java
class Link
   {
   public long dData;
   public Link next;

   public Link(long d)               
      { dData = d; }

   public void displayLink()         
      { System.out.print(dData + " "); }

   }  // end class Link

class FirstLastList
   {
   private Link first;               
   private Link last;

   public FirstLastList()
      {
      first = null;                  
      last = null;
      }

   public boolean isEmpty()          
      { return first==null; }

   public void insertLast(long dd) 
      {
      Link newLink = new Link(dd);   
      if( isEmpty() ) 
         first = newLink;    
      else
         last.next = newLink; 
      last = newLink; 
      }

   public long deleteFirst()         
      {          
      long temp = first.dData;
      if(first.next == null)         
         last = null;     
      first = first.next;
      return temp;
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

class LinkQueue
   {
   private FirstLastList theList;

   public LinkQueue()
      { theList = new FirstLastList(); }

   public boolean isEmpty()
      { return theList.isEmpty(); }

   public void insert(long j)
      { theList.insertLast(j); }

   public long remove() 
      {  return theList.deleteFirst();  }

   public void displayQueue()
      {
      System.out.print("Queue (front-->rear): ");
      theList.displayList();
      }

   }  

class LinkQueueApp
   {
   public static void main(String[] args)
      {
      LinkQueue theQueue = new LinkQueue();
      theQueue.insert(20);                 
      theQueue.insert(40);

      theQueue.displayQueue();             

      theQueue.insert(60);                 
      theQueue.insert(80);

      theQueue.displayQueue();             

      theQueue.remove();                   
      theQueue.remove();

      theQueue.displayQueue();   
      }  
   }  
```