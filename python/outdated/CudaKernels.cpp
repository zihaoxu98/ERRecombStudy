#include <stdio.h>

///////////
// utils //
///////////
__device__ float* copy_farray(float *arr, int length, float *cache);
__device__ int*   copy_iarray(int *arr, int length, int *cache);
__device__ void   set_farray_values(float *arr, float *values, int length);
__device__ void   set_iarray_values(int *arr, int *values, int length);
__device__ void   apply_map_to_farray(float *arr, int *map, int length, float *cache);
__device__ void   apply_map_to_iarray(int *arr, int *map, int length, int *cache);
__device__ void   print_farr(float *arr, int length);
__device__ void   print_iarr(int *arr, int length);
__device__ void   invert_map(int *map, int length, int *cache);

//////////
// sort //
//////////
__device__ void   fswap(float *a, float *b);
__device__ void   iswap(int *a, int *b);

__device__ int    _quick_sort_iarr_by_fkey_partition(int *arr, float *key, int left, int right);
__device__ int    _quick_sort_iarr_by_ikey_partition(int *arr, int *key, int left, int right);
__device__ void   _quick_sort_iarr_by_fkey(int *arr, float *key, int left, int right);
__device__ void   _quick_sort_iarr_by_ikey(int *arr, int *key, int left, int right);
__device__ void   quick_sort_iarr_by_fkey(int *arr, float *key, int length);
__device__ void   quick_sort_iarr_by_ikey(int *arr, int *key, int length);

__device__ void   bubble_sort_iarr_by_fkey(int *arr, float *key, int length);
__device__ void   bubble_sort_iarr_by_ikey(int *arr, int *key, int length);

__device__ int*   arg_fsort(float *arr, int length, int *map, float *cache);
__device__ int*   arg_isort(int *arr, int length, int *map, int *cache);

///////////
// utils //
///////////
__device__ float* copy_farray(float *arr, int length, float *cache)
{
    float *cpy = cache;
    
    for(int i = 0; i < length; i++)
        cpy[i] = arr[i];
    
    return cpy;
}

__device__ int* copy_iarray(int *arr, int length, int *cache)
{
    int *cpy = cache;
    
    for(int i = 0; i < length; i++)
        cpy[i] = arr[i];
    
    return cpy;
}

__device__ void set_farray_values(float *arr, float *values, int length)
{
    for(int i = 0; i < length; i++) arr[i] = values[i];
}

__device__ void set_iarray_values(int *arr, int *values, int length)
{
    for(int i = 0; i < length; i++) arr[i] = values[i];
}

__device__ void apply_map_to_farray(float *arr, int *map, int length, float *cache)
{
    float *arr0 = copy_farray(arr, length, cache);
    for(int i = 0; i < length; i++) arr[i] = arr0[map[i]];
}

__device__ void apply_map_to_iarray(int *arr, int *map, int length, int *cache)
{
    int *arr0 = copy_iarray(arr, length, cache);
    for(int i = 0; i < length; i++) arr[i] = arr0[map[i]];
}

__device__ void print_farr(float *arr, int length)
{
    for(int i = 0; i < length; i++) printf("%f, ", arr[i]);
    printf("\n");
}

__device__ void print_iarr(int *arr, int length)
{
    for(int i = 0; i < length; i++) printf("%d, ", arr[i]);
    printf("\n");
}

__device__ void invert_map(int *map, int length, int *cache)
{
    // invert the permutation map
    int *map0 = copy_iarray(map, length, cache);
    for(int i = 0; i < length; i++) map[map0[i]] = i;
}

//////////
// sort //
//////////
__device__ void fswap(float *a, float *b)
{
    float t = *a;
    *a = *b;
    *b = t;
}

__device__ void iswap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

__device__ int _quick_sort_iarr_by_fkey_partition(int *arr, float *key, int left, int right)
{
    float pivot = key[right];
    int i = left - 1;
    for(int j = left; j < right; j++)
    {
        if(key[j] <= pivot)
        {
            i++;
            iswap(&arr[i], &arr[j]);
            fswap(&key[i], &key[j]);
        }
    }
    iswap(&arr[i+1], &arr[right]);
    fswap(&key[i+1], &key[right]);
    return i+1;
}

__device__ int _quick_sort_iarr_by_ikey_partition(int *arr, int *key, int left, int right)
{
    float pivot = key[right];
    int i = left - 1;
    for(int j = left; j < right; j++)
    {
        if(key[j] <= pivot)
        {
            i++;
            iswap(&arr[i], &arr[j]);
            iswap(&key[i], &key[j]);
        }
    }
    iswap(&arr[i+1], &arr[right]);
    iswap(&key[i+1], &key[right]);
    return i+1;
}

__device__ void _quick_sort_iarr_by_fkey(int *arr, float *key, int left, int right)
{
    if(left < right)
    {
        int p = _quick_sort_iarr_by_fkey_partition(arr, key, left, right);
        _quick_sort_iarr_by_fkey(arr, key, left, p-1);
        _quick_sort_iarr_by_fkey(arr, key, p+1, right);
    }
}

__device__ void _quick_sort_iarr_by_ikey(int *arr, int *key, int left, int right)
{
    if(left < right)
    {
        int p = _quick_sort_iarr_by_ikey_partition(arr, key, left, right);
        _quick_sort_iarr_by_ikey(arr, key, left, p-1);
        _quick_sort_iarr_by_ikey(arr, key, p+1, right);
    }
}

__device__ void quick_sort_iarr_by_fkey(int *arr, float *key, int length)
{
    _quick_sort_iarr_by_fkey(arr, key, 0, length-1);
}

__device__ void quick_sort_iarr_by_ikey(int *arr, int *key, int length)
{
    _quick_sort_iarr_by_ikey(arr, key, 0, length-1);
}

__device__ void bubble_sort_iarr_by_fkey(int *arr, float *key, int length)
{
    for(int i = 0; i < length-1; i++)
        for(int j = 0; j < length-1-i; j++)
            if(key[j] > key[j+1])
            {
                iswap(&arr[j], &arr[j+1]);
                fswap(&key[j], &key[j+1]);
            }
    
}
__device__ void bubble_sort_iarr_by_ikey(int *arr, int *key, int length)
{
    for(int i = 0; i < length-1; i++)
        for(int j = 0; j < length-1-i; j++)
            if(key[j] > key[j+1])
            {
                iswap(&arr[j], &arr[j+1]);
                iswap(&key[j], &key[j+1]);
            }
}

__device__ int* arg_fsort(float *arr, int length, int *map, float *cache)
{
    float *key = copy_farray(arr, length, cache);
    for(int i = 0; i < length; i++) map[i] = i;
    bubble_sort_iarr_by_fkey(map, key, length); // on gpu we use bubble sort not quick sort
    return map;
}
__device__ int* arg_isort(int *arr, int length, int *map, int *cache)
{
    int *key = copy_iarray(arr, length, cache);
    for(int i = 0; i < length; i++) map[i] = i;
    bubble_sort_iarr_by_ikey(map, key, length); // on gpu we use bubble sort not quick sort
    return map;
}

//////////
// main //
//////////
__global__ void calculate_recomb(
    float *X,
    float *Y,
    float *Ed,
    int *event_end,
    int *num_event,
    float *params,
    float *X_edge,
    float *Y_edge,
    int *X_edge_ID,
    int *Y_edge_ID,
    float *weight,
    float *fcache,
    int *icache,
    float *recomb
)
{
    int event_id = threadIdx.x;
    if(event_id >= *num_event) return;
    
    int left = event_end[event_id];
    int right = event_end[event_id+1];
    int length = right - left;
    int num_grid = 2 * length - 1;
    
    // make offsets
    X = &X[left];
    Y = &Y[left];
    Ed = &Ed[left];
    X_edge = &X_edge[left*2];
    Y_edge = &Y_edge[left*2];
    X_edge_ID = &X_edge_ID[left*2];
    Y_edge_ID = &Y_edge_ID[left*2];
    int offset = 0;
    for(int i = 0; i < event_id; i++)
    {
        int prev_num_grid = 2*(event_end[i+1] - event_end[i])-1;
        offset += prev_num_grid*prev_num_grid;
    }
    weight = &weight[offset];
    fcache = &fcache[left*4];
    icache = &icache[left*4];

    // model parameters
    float alpha = params[0];
    float beta = params[1];
    
    for(int i = 0; i < length; i++)
    {
        X_edge[i*2] = X[i] - alpha;
        X_edge[i*2+1] = X[i] + alpha;
        Y_edge[i*2] = Y[i] - alpha;
        Y_edge[i*2+1] = Y[i] + alpha;
        X_edge_ID[i*2] = X_edge_ID[i*2+1] = i;
        Y_edge_ID[i*2] = Y_edge_ID[i*2+1] = i;
    }
    
    
    int *map = arg_fsort(X_edge, length*2, icache, fcache);
    apply_map_to_farray(X_edge, map, length*2, fcache);
    apply_map_to_iarray(X_edge_ID, map, length*2, &icache[2*length]);
    int *tmp = arg_isort(X_edge_ID, length*2, icache, &icache[2*length]);
    set_iarray_values(X_edge_ID, tmp, length*2);
    for(int i = 0; i < length; i++)
        if(X_edge_ID[i*2] > X_edge_ID[i*2+1])
            iswap(&X_edge_ID[i*2], &X_edge_ID[i*2+1]);
            
    map = arg_fsort(Y_edge, length*2, icache, fcache);
    apply_map_to_farray(Y_edge, map, length*2, fcache);
    apply_map_to_iarray(Y_edge_ID, map, length*2, &icache[2*length]);
    tmp = arg_isort(Y_edge_ID, length*2, icache, &icache[2*length]);
    set_iarray_values(Y_edge_ID, tmp, length*2);
    for(int i = 0; i < length; i++)
        if(Y_edge_ID[i*2] > Y_edge_ID[i*2+1])
            iswap(&Y_edge_ID[i*2], &Y_edge_ID[i*2+1]);

    float energy = 0;
    for(int i = 0; i < length; i++)
    {
        int x_left = X_edge_ID[i*2];
        int x_right = X_edge_ID[i*2+1];
        int y_left = Y_edge_ID[i*2];
        int y_right = Y_edge_ID[i*2+1];
        float density = Ed[i] / 4.0 / alpha / alpha;
        energy += Ed[i];
        
        for(int indx = x_left; indx < x_right; indx++)
            for(int indy = y_left; indy < y_right; indy++)
                weight[indx + indy * num_grid] += density;
    }
    
    
    for(int i = 0; i < num_grid*num_grid; i++)
    {
        if(beta * weight[i] < 1e-5) weight[i] = weight[i] / energy;
        else weight[i] = logf(1.0 + weight[i] * beta) / beta / energy;
    }
    
    // test
    for(int indx = 0; indx < num_grid; indx++)
        for(int indy = 0; indy < num_grid; indy++)
            weight[indx + indy * num_grid] *= (X_edge[indx+1]-X_edge[indx])*(Y_edge[indy+1]-Y_edge[indy]);

    float r = 0;
    for(int i = 0; i < num_grid*num_grid; i++)
        r += weight[i];
    recomb[event_id] = 1 - r;
    
//     printf("%d\t%f\n", event_id, r);
}
