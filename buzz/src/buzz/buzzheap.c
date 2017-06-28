#include "buzzheap.h"
#include "buzzvm.h"
#include <stdio.h>
#include <stdlib.h>

/****************************************/
/****************************************/

#define BUZZHEAP_GC_INIT_MAXOBJS 1

/****************************************/
/****************************************/

void buzzheap_destroy_obj(uint32_t pos, void* data, void* params) {
   buzzobj_destroy((buzzobj_t*)data);
}

buzzheap_t buzzheap_new() {
   /* Create heap state */
   buzzheap_t h = (buzzheap_t)malloc(sizeof(struct buzzheap_s));
   /* Create object list */
   h->objs = buzzdarray_new(10, sizeof(buzzobj_t), buzzheap_destroy_obj);
   /* Initialize GC max object threshold */
   h->max_objs = BUZZHEAP_GC_INIT_MAXOBJS;
   /* Initialize the marker */
   h->marker = 0;
   /* All done */
   return h;
}

/****************************************/
/****************************************/

void buzzheap_destroy(buzzheap_t* h) {
   /* Get rid of object list */
   buzzdarray_destroy(&((*h)->objs));
   /* Get rid of heap state */
   free(*h);
   /* Set heap to NULL */
   *h = NULL;
}

/****************************************/
/****************************************/

buzzobj_t buzzheap_newobj(buzzheap_t h,
                          uint16_t type) {
   /* Create a new object. calloc() fills it with zeroes */
   buzzobj_t o = buzzobj_new(type);
   /* Set the object marker */
   o->o.marker = h->marker;
   /* Add object to list */
   buzzdarray_push(h->objs, &o);
   /* All done */
   return o;
}

/****************************************/
/****************************************/

static void buzzheap_objmark(buzzobj_t o, buzzvm_t h);
static void buzzheap_darrayobj_mark(uint32_t pos, void* data, void* params);
static void buzzheap_dictobj_mark(const void* key, void* data, void* params);

void buzzheap_objmark(buzzobj_t o,
                      buzzvm_t vm) {
   /*
    * Nothing to do if the object is already marked
    * This avoids infinite looping when cycles are present
    */
   if(o->o.marker == vm->heap->marker) return;
   /* Update marker */
   o->o.marker = vm->heap->marker;
   /* Take care of composite types */
   if(o->o.type == BUZZTYPE_TABLE)
      buzzdict_foreach(o->t.value,
                       buzzheap_dictobj_mark,
                       vm);
   else if(o->o.type == BUZZTYPE_CLOSURE)
      buzzdarray_foreach(o->c.value.actrec,
                         buzzheap_darrayobj_mark,
                         vm);
   else if(o->o.type == BUZZTYPE_STRING)
      buzzstrman_gc_mark(vm->strings,
                         o->s.value.sid);
}

void buzzheap_dictobj_mark(const void* key, void* data, void* params) {
   buzzheap_objmark(*(buzzobj_t*)key, params);
   buzzheap_objmark(*(buzzobj_t*)data, params);
}

void buzzheap_darrayobj_mark(uint32_t pos, void* data, void* params) {
   buzzheap_objmark(*(buzzobj_t*)data, (buzzvm_t)params);
}

void buzzheap_stack_mark(uint32_t pos, void* data, void* params) {
   buzzdarray_foreach(*(buzzdarray_t*)data,
                      buzzheap_darrayobj_mark,
                      params);
}

void buzzheap_lsyms_mark(uint32_t pos, void* data, void* params) {
   buzzdarray_foreach((*(buzzvm_lsyms_t*)data)->syms,
                      buzzheap_darrayobj_mark,
                      params);
}

void buzzheap_vstigobj_mark(const void* key, void* data, void* params) {
   buzzheap_objmark((*(buzzobj_t*)key), params);
   buzzheap_objmark((*(buzzvstig_elem_t*)data)->data, params);
}

void buzzheap_vstig_mark(const void* key, void* data, void* params) {
   buzzvstig_foreach(*(buzzvstig_t*)data,
                     buzzheap_vstigobj_mark,
                     params);
}

void buzzheap_listener_mark(const void* key, void* data, void* params) {
   buzzstrman_gc_mark(((buzzvm_t)params)->strings, *(uint16_t*)key);
   buzzheap_objmark(*(buzzobj_t*)data, params);
}

void buzzheap_gsymobj_mark(const void* key, void* data, void* params) {
   buzzheap_objmark(*(buzzobj_t*)data, params);
}

void buzzheap_gc(struct buzzvm_s* vm) {
   buzzheap_t h = vm->heap;
   /* Is GC necessary? */
   if(buzzdarray_size(h->objs) < h->max_objs) return;
   /* Increase the marker */
   ++h->marker;
   /* Prepare string gc */
   buzzstrman_gc_clear(vm->strings);
   /* Go through all the objects in the global symbols and mark them */
   buzzdict_foreach(vm->gsyms, buzzheap_gsymobj_mark, vm);
   /* Go through all the objects in the VM stack and mark them */
   buzzdarray_foreach(vm->stacks, buzzheap_stack_mark, vm);
   /* Go through all the objects in the local symbol stack and mark them */
   buzzdarray_foreach(vm->lsymts, buzzheap_lsyms_mark, vm);
   /* Go through all the objects in the virtual stigmergy and mark them */
   buzzdict_foreach(vm->vstigs, buzzheap_vstig_mark, vm);
   /* Go through all the objects in the listeners and mark them */
   buzzdict_foreach(vm->listeners, buzzheap_listener_mark, vm);
   /* Go through all the objects in the out message queue and mark them */
   buzzoutmsg_gc(vm);
   /* Go through all the objects in the object list and delete the unmarked ones */
   int64_t i = buzzdarray_size(h->objs) - 1;
   while(i >= 0) {
      /* Check whether the marker is set to the latest value */
      if(buzzdarray_get(h->objs, i, buzzobj_t)->o.marker != h->marker) {
         /* No, erase the element */
         buzzdarray_remove(h->objs, i);
      }
      /* Next element */
      --i;
   }
   /* Perform string gc */
   buzzstrman_gc_prune(vm->strings);
   /* Update the max objects threshold */
   h->max_objs = buzzdarray_isempty(h->objs) ? BUZZHEAP_GC_INIT_MAXOBJS : 2 * buzzdarray_size(h->objs);
}

/****************************************/
/****************************************/
