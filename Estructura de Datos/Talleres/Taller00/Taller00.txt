/* Julian Perez
Daniel Galviz
Sebastian Rodriguez
David Rodriguez*/


#include <iostream>
#include <ctime>
#include <fstream>

using namespace std;

//Funcion que retorna numero aleatorio entre el 0 y el 25
int aleatorio(){
  return rand()%26;
}

  struct factura{
  string nombre;
  string nombreProducto;
  int cedula;
  int cantidad;
  double precio;
  };

//Funcion para crear fichero en un txt
void ImpresionFichero(factura *p){
  
 ofstream F( "factura.txt");
  if (F.is_open()){
  F<<endl<<endl<<"Ingrese el nombre del cliente: "<<(*p).nombre<<endl<<endl;
  F<<"Ingrese el nombre del producto: "<<(*p).nombreProducto<<endl<<endl;
  F<<"Ingrese el numero de cedula: "<<(*p).cedula<<endl<<endl;
  F << "Cantidad de productos: "<<(*p).cantidad<<endl<<endl;
  F << "Precio del producto: "<<(*p).precio<<endl<<endl;
  F << "Valor a pagar: " << (*p).cantidad*(*p).precio << endl<<endl;
    
    F.close();
}else{
    cout<<"No se pudo abrir el archivo"<<endl<<endl;
}


}
  

int main() {
  //Se crea un arreglo con 10 elementos y un apuntador
  int numeros[10];
  int *ptr;

  //Creacion de la semilla
  srand(time(0));

  //Se llena el arreglo con numeros aleatorios
  for(int i=0; i<10; i++){
    numeros[i]=aleatorio();
  }
  //Se inicializa el apuntador en la direccion del primer elemento del arreglo
  ptr=numeros;

  //Se imprime el arreglo
  for(int i=0; i<10; i++){
    cout<<*(ptr+i)<<" "; 
  }

  cout<<endl<<endl;

  //Se imprime la direccion de memoria de cada elemento del arreglo
  for(int i=0; i<10; i++){
    cout<<(ptr+i)<<endl<<endl; 
  }

  //Ejercicio 2

  cout<<"Ejercicio 2"<<endl;

  //Datos de factura encapsulados, apuntados por apuntador
  factura *puntero =new factura;

  //Captura de datos del cliente y factura
  cout<<"Ingrese el nombre del cliente: ";
  cin>>(*puntero).nombre;
  cout<<"Ingrese el nombre del producto: ";
  cin>>(*puntero).nombreProducto;
  cout<<"Ingrese el numero de cedula: ";
  cin>>(*puntero).cedula;
  cout << "Cantidad de productos: ";
  cin >> (*puntero).cantidad;
  cout << "Precio del producto: ";
  cin >> (*puntero).precio;



  //Impresion para generar el fichero de texto
  cout<<endl<<endl<<"Ingrese el nombre del cliente: "<<(*puntero).nombre<<endl<<endl;
  cout<<"Ingrese el nombre del producto: "<<(*puntero).nombreProducto<<endl<<endl;
  cout<<"Ingrese el numero de cedula: "<<(*puntero).cedula<<endl<<endl;
  cout << "Cantidad de productos: "<<(*puntero).cantidad<<endl<<endl;
  cout << "Precio del producto: "<<(*puntero).precio<<endl<<endl;
  cout << "Valor a pagar: " << (*puntero).cantidad*(*puntero).precio << endl<<endl;

  ImpresionFichero(puntero);
  
  
  
return 0;



}
