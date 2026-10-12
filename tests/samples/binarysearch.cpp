
#include <iostream>
using namespace std;

int main()
{
    int arr[8] = {2, 5, 8, 12, 16, 23, 38, 56};
    int key = 23;

    int low = 0;
    int high = 7;
    int mid;

    while (low <= high)
    {
        mid = (low + high) / 2;

        if (arr[mid] == key)
            break;
        else if (arr[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return 0;
}
