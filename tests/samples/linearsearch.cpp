
#include <iostream>
using namespace std;

int main()
{
    int arr[] = {10, 20, 30, 40, 50};
    int n = 5;
    int key = 30;
    int found = 0;

    for (int i = 0; i < n; i++)
    {
        if (arr[i] == key)
        {
            cout << "Element found at index " << i << endl;
            found = 1;
            break;
        }
    }

    if (found == 0)
    {
        cout << "Element not found" << endl;
    }

    return 0;
}
