var list = new Array(100);
for(i = 0; i < list.length; i++){
    list[i] = Math.floor(Math.random() * 100);
}
function printList(list){
    for(i = 0; i < list.length; i++){
        console.log(list[i]);
    }
}
function partition(list, left, right){
    var pivot = list[left];
    var i = left;
    var j = right;
    while (true){
        while(list[i] < pivot){
            i++;
        }
        while(list[j] > pivot){
            j--;
        }
        if (i >= j) return j
        else{
            var temp = list[i];
            list[i] = list[j];
            list[j] = temp;
            i++;
            j--;
        }
    }
}
function quicksort(list, left, right){
    if(left < right){
        var p = partition(list, left, right);
        quicksort(list, left, p);
        quicksort(list, p+1, right);
    }
}
function quick(list){
    quicksort(list, 0, list.length-1);
}
quick(list);
printList(list)