#include "mem/ruby/structures/Predictor.hh"

#include "debug/RubyPredictor.hh"

Predictor*
PredictorParams::create()
{
    return new Predictor(this);
}

Predictor::Predictor(const Params *p)
    : SimObject(p), p_num_histories(p->num_histories),
      p_num_patterns(p->num_patterns)
{
    assert(p_num_histories > 0);
    assert(p_num_patterns > 0);

    p_array.resize(p_num_histories);
    for (int i=0;i<p_num_histories;i++) {
      p_array[i] = HistoryTable();
    }

}

Predictor::~Predictor()
{

}

void
Predictor::regStats()
{
    SimObject::regStats();

    numReads
      .name(name() + ".reads_observed")
      .desc("number of reads observed")
      ;

    numWrites
      .name(name() + ".writes_observed")
      .desc("number of writes observed")
      ;
}

void
Predictor::observeActivity(Addr address, const MachineID& requester,
                           const CoherenceRequestType& type, int accessType)
{
    DPRINTF(RubyPredictor, "Observed activity for %#x\n", address);
    Addr line_addr = makeLineAddress(address);

    // check if the address already exists in the history table
    HistoryTable *entry = getHistoryEntry(line_addr);
    if (entry != NULL) {
        // TODO: add the new activity to the pattern table
        // it might go over limit of num_patterns
        // entry->h_array[entry->cur_num_pattern++].i_sender = requester;
        // entry->h_array[0].i_type = type;

        // update the value of sender and type
        entry->h_sender = requester;
        entry->h_type = type;

        // update the value of read and write based on the request type
        if (accessType)
            entry->h_writes += 1;
        else
            entry->h_reads += 1;
        //we don't really need it?
        //cuz we need to allocate new entry for every access?
        numReads = entry->h_reads;
        numWrites = entry->h_writes;
        return;
    }

    allocateNewEntry(getLRUindex(), line_addr, requester, type, accessType);
    return;
}

uint32_t
Predictor::getLRUindex(void)
{
    uint32_t index = 0;
    Cycles lru_access = p_array[index].h_use_time;

    for (uint32_t i = 0; i < p_num_histories; ++i) {
        if (!p_array[i].h_is_valid) {
            return i;
        }

        if (p_array[i].h_use_time < lru_access) {
            lru_access = p_array[i].h_use_time;
            index = i;
        }
    }

    return index;
}

void
Predictor::allocateNewEntry(uint32_t index, Addr address,
                            const MachineID& requester,
                            const CoherenceRequestType& type,
                            int accessType)
{
    HistoryTable *entry = &(p_array[index]);
    entry->h_address = makeLineAddress(address);
    entry->h_is_valid = true;

    entry->h_sender = requester;
    entry->h_type = type;

    entry->h_writes = 0;
    entry->h_reads = 0;

    // Set the value of read and write based on the request type
    if (accessType)
        entry->h_writes += 1;
    else
        entry->h_reads += 1;

    // TODO: create pattern table
    // entry->h_array =vector<PatternTable>();
    // entry->h_array.resize(p_num_patterns);
    // entry->h_array[0].p_is_valid = true;
    // entry->h_array[0].i_sender = requester;
    // entry->h_array[0].i_type = type;
    // entry->cur_num_pattern=1;

}

HistoryTable*
Predictor::getHistoryEntry(Addr address)
{
    for (int i = 0; i < p_num_histories; ++i) {
        if (p_array[i].h_is_valid && p_array[i].h_address == address)
            return &(p_array[i]);
    }

    return NULL;
}
