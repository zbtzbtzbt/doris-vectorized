// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#ifndef DORIS_AGGREGATE_FUNCTION_NDV_H
#define DORIS_AGGREGATE_FUNCTION_NDV_H
#include <iostream>
typedef unsigned short uint32_t;
typedef unsigned char uint8_t;
namespace doris::vectorized {
    class SampledNdvState {
    public:
        /// Empirically determined number of HLL buckets. Power of two for fast modulo.
        static const uint32_t NUM_HLL_BUCKETS = 32;
        
        /// A bucket contains an update count and an HLL intermediate state.
        static constexpr int64_t BUCKET_SIZE =
                sizeof(int64_t) + AggregateFunctions::DEFAULT_HLL_LEN;
        
        /// Sampling percent which was given as the second argument to SampledNdv().
        /// Stored here to avoid existing issues with passing constant arguments to all
        /// aggregation phases and because we convert the sampling percent argument from
        /// decimal to double. See IMPALA-6179.
        double sample_perc;
        
        /// Counts the number of Update() calls. Used for determining which bucket to update.
        int64_t total_row_count;
        
        /// Array of buckets.
        struct {
            int64_t row_count;
            /// DEFAULT_HLL_LEN Todo,use var to change
            uint8_t hll[1<<10];
        } buckets[NUM_HLL_BUCKETS];
    };
    
    template <typename T>
    struct AggregateFunctionNDVData {
        T sum{};
    
        
        void add(T value) { sum += value; }
    
        void merge(const AggregateFunctionSumData& rhs) { sum += rhs.sum; }
    
        void write(BufferWritable& buf) const { write_binary(sum, buf); }
    
        void read(BufferReadable& buf) { read_binary(sum, buf); }
    
        T get() const { return sum; }
};

} // namespace doris::vectorized

#endif //DORIS_AGGREGATE_FUNCTION_NDV_H
