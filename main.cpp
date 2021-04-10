#include <iostream>
#include <vector>
using namespace std;

typedef unsigned int uint;

struct product{
    unsigned int weight;
    unsigned int resistance;
};


// flag podas
bool optimalidad = false;
bool factibilidad = true;

// jambotubo
unsigned int quantity_products;
unsigned int resistance; // resistencia total del tubo

vector<bool> mask_products;
vector<product> products;
unsigned int result;

unsigned int partial_products = 0;

// aux: despues borrar, es solo para pruebas
int aux_count = 0;

// O(n)
unsigned int quantity_products_jambotubo(){
    unsigned int acum = 0;
    for (int i = 0; i < quantity_products; ++i) {
        if (mask_products[i]){
            acum ++;
        }
    }
    return acum;
}


// O(n)
bool jambotube_broke(int n){
    unsigned int local_resistance = 0;
    unsigned int local_weight = 0;
    unsigned int total_weight = 0;
    vector<product> partial_products;

    // estas 2 variables despues borrar, son para poder debugear estas variables locales nomas
    vector<bool> mask_products_copy = mask_products;
    vector<product> products_copy = products;

    //creo un vector partial_products que va a contener <resistencia, peso acumulado> con todos los productos que tengo puestos
    for (int i = 0; i < n ; ++i) {
        if (mask_products[i]) {
            total_weight += products[i].weight;
            product actual_product = products[i];
            actual_product.weight = total_weight;
            partial_products.push_back(actual_product);
        }
    }

    // me fijo que cada producto no se haya roto, hago (resistencia < peso final - peso que esta por debajo de ese producto)
    for (int i = 0; i < partial_products.size() ; ++i) {
        local_resistance = partial_products[i].resistance;
        local_weight = partial_products[i].weight;
        if (local_resistance < total_weight - local_weight){
            return true;
        }
    }

    // verificar si el peso total rompe al tubo en si
    if (total_weight > resistance){
        return true;
    }

    return false;
}


// O( 2n.(2^n) )  =  O( n . (2^n) )  (lo unico, ver bien el tema de que la complejidad sea con la multiplicación)
void brute_force(int i = 0){

    aux_count ++;

    if (i == quantity_products){

        // estas 3 variables despues borrar, son para poder debugear estas variables locales nomas
        vector<bool> mask_products_copy = mask_products;
        vector<product> products_copy = products;
        unsigned int result_copy = result;

        cout << jambotube_broke(i) << endl;  // borrar despues

        // O(2.n)
        if (not jambotube_broke(i)){                                         //O(n)
            unsigned int local_result = 0;
            local_result = quantity_products_jambotubo();                   //O(n)
            result = (local_result > result)? local_result : result;
        }

    } else {

        // no se si hay un = mask_products[i] = false; de mas (posiblemente si)
        mask_products[i] = false;
        brute_force(i + 1);

        mask_products[i] = true;
        brute_force(i + 1);
    }
}


void backtracking(int i = 0){

    // solo para ver de antemano que tan optimo parece ser, despues se borra
    aux_count ++;

    // estas 3 variables despues borrar, son para poder debugear estas variables locales nomas
    vector<bool> mask_products_copy = mask_products;
    vector<product> products_copy = products;
    unsigned int result_copy = result;
    unsigned int partial_products_copy = partial_products;
    unsigned  int quantity_products_copy = quantity_products;


//    //poda factibilidad O(n)
    if (factibilidad) {
        if (jambotube_broke(i)) return;
    }

    // optimalidad me huele raro
    //poda optimalidad O(1)
    if (optimalidad) {
        unsigned int max_possible_res = partial_products + quantity_products - i;
        if (max_possible_res <= result) return;
    }


    if (i == quantity_products){

        // O(2.n)
        if (not jambotube_broke(i)){                                         //O(n)
            unsigned int local_result = 0;
            local_result = quantity_products_jambotubo();                   //O(n)
            result = (local_result > result)? local_result : result;
        }

    } else {

        // no se si hay un = mask_products[i] = false; de mas (posiblemente si)
        mask_products[i] = false;
        backtracking(i + 1);

        partial_products ++;
        mask_products[i] = true;
        backtracking(i + 1);
    }
}


int main() {

    // pedir datos
    cin >> quantity_products;
    cin >> resistance;
    for (int i = 0; i < quantity_products; ++i) {
        unsigned int weight;
        unsigned int resistance;
        cin >> weight;
        cin >> resistance;
        product actual_product;
        actual_product.weight = weight;
        actual_product.resistance = resistance;
        products.push_back(actual_product);
        mask_products.push_back(false);
    }

//    brute_force();
//    cout << "result: "<< result << endl;

    backtracking();
    cout << "result: "<< result << endl;

    cout << "entro: "<< aux_count << " veces"<< endl;



    return 0;
}

/*  

5 50
10 45
20 8
30 15
10 2
15 30

20 50
10 45
20 8
30 15
10 2
15 30
10 45
20 8
30 15
10 2
15 30
10 45
20 8
30 15
10 2
15 30
10 45
20 8
30 15
10 2
15 30

 result: 5
entro: 2097151 veces


 */

