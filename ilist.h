typedef struct {
    short signed int next, prev;
} __attribute__((aligned(2 * sizeof(short signed int)))) ilist_head;

#define ILIST_INIT(P)              \
    do {                           \
        (P)->next = (P)->prev = 0; \
    } while (0)

#define ilist_is_first(P, H) ((P) == &(H)[(H)->next])

#define ilist_is_last(P, H) ((P) == &(H)[(H)->prev])

#define ilist_entry(P, T, M) ((T *)((char *)(P) - __builtin_offsetof(T, M)))

#define ilist_first_entry(P, T, M) ilist_entry(&(P)[(P)->next], T, M)

#define ilist_last_entry(P, T, M) ilist_entry(&(P)[(P)->prev], T, M)

#define ilist_next_entry(P, M) ilist_entry(&(P)->M + (P)->M.next, __typeof__(*P), M)

#define ilist_prev_entry(P, M) ilist_entry(&(P)->M + (P)->M.prev, __typeof__(*P), M)

#define ilist_for_each_entry(P, H, M) \
    for (P = ilist_first_entry(H, __typeof__(*P), M); &P->M != (H); P = ilist_next_entry(P, M))

#define ilist_empty(H)       ((H)->next == 0 && (H)->prev == 0)
#define ilist_is_singular(H) ((H)->next == (H)->prev && (H)->next != 0)

static inline void ilist_add_(ilist_head *h, ilist_head *prev, ilist_head *next);
static inline void ilist_add(ilist_head *h, ilist_head *head);
static inline void ilist_add_tail(ilist_head *h, ilist_head *head);
static inline void ilist_add_before_tail(ilist_head *h, ilist_head *head);
static inline void ilist_del_(ilist_head *prev, ilist_head *next);
static inline void ilist_del(ilist_head *old);

//
// Subtract two pointers and return the difference as short int.
// Check for overflow.
//
static inline short signed int safe_subtract(ilist_head *a, ilist_head *b)
{
    ssize_t diff = a - b;

    if (diff > 0x7fff || diff < -0x8000) {
        std::stringstream ss;
        ss << "pointer " << a << " is too far from " << b;
        throw std::runtime_error(ss.str());
    }
    return diff;
}

static inline void ilist_add_(ilist_head *h, ilist_head *prev, ilist_head *next)
{
    next->prev = safe_subtract(h, next);
    h->next    = safe_subtract(next, h);
    h->prev    = safe_subtract(prev, h);
    prev->next = safe_subtract(h, prev);
}

static inline void ilist_add(ilist_head *h, ilist_head *head)
{
    ilist_add_(h, head, &head[head->next]);
}

static inline void ilist_add_tail(ilist_head *h, ilist_head *head)
{
    ilist_add_(h, &head[head->prev], head);
}

static inline void ilist_add_before_tail(ilist_head *h, ilist_head *head)
{
    head = head + head->prev;
    ilist_add_(h, &head[head->prev], head);
}

static inline void ilist_del_(ilist_head *prev, ilist_head *next)
{
    next->prev = prev - next;
    prev->next = next - prev;
}

static inline void ilist_del(ilist_head *old)
{
    ilist_del_(&old[old->prev], &old[old->next]);
    old->next = old->prev = 0;
}
