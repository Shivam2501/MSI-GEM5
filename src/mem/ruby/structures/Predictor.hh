#ifndef __MEM_RUBY_STRUCTURES_PREDICTOR_HH__
#define __MEM_RUBY_STRUCTURES_PREDICTOR_HH__

using namespace std;

#include "mem/protocol/CoherenceRequestType.hh"
#include "mem/ruby/common/Address.hh"
#include "mem/ruby/common/MachineID.hh"
#include "mem/ruby/slicc_interface/AbstractController.hh"
#include "mem/ruby/slicc_interface/RubyRequest.hh"
#include "params/Predictor.hh"
#include "sim/sim_object.hh"
#include "sim/system.hh"

class PatternTable
{
    public:
        PatternTable()
        {
            p_is_valid = false;
        }

        // Index
        MachineID i_sender;
        CoherenceRequestType i_type;

        // Prediction
        MachineID p_receiver;
        CoherenceRequestType p_type;

        bool p_is_valid;
};

class HistoryTable
{
    public:
        HistoryTable()
        {
            h_is_valid = false;
            h_use_time = Cycles(0);
            cur_num_pattern = 0;
        }

        Addr h_address;

        // Last time when the access was made to the address
        Cycles h_use_time;

        // History Register
        MachineID h_sender;
        CoherenceRequestType h_type;

        // Pattern History Table
        // vector<PatternTable> h_array;

        // Access Counters
        uint32_t h_reads;
        uint32_t h_writes;

        bool h_is_valid;
        uint32_t cur_num_pattern;
};

class Predictor : public SimObject
{
    public:
        typedef PredictorParams Params;
        Predictor(const Params *p);
        ~Predictor();

        // Functions called by the directory controller
        void observeActivity(Addr address, const MachineID& requester,
                             const CoherenceRequestType& type, int accessType);

        // Set the directory controller
        void setController(AbstractController *_ctrl)
        { p_controller = _ctrl; }

        void regStats();

    private:

        // Returns an unused history based a lru policy
        uint32_t getLRUindex(void);

        // Creates a new entry in the history table
        void allocateNewEntry(uint32_t index, Addr address,
                              const MachineID& requester,
                              const CoherenceRequestType& type,
                              int accessType);

        uint32_t p_num_histories;
        uint32_t p_num_patterns;

        // Check if they address exists in the history table
        // return a pointer to the matching entry or NULL if not found
        HistoryTable* getHistoryEntry(Addr address);

        vector<HistoryTable> p_array;

        AbstractController *p_controller;

        Stats::Scalar numReads;
        Stats::Scalar numWrites;
};

#endif // __MEM_RUBY_STRUCTURES_PREDICTOR_HH__
