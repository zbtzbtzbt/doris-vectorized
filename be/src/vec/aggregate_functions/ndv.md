---
{
"title": "NDV",
"language": "zh-CN"
}
---

<!-- 
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
-->

# NDV
## description
### Syntax

```
NDV(col)

NDV(col_1,col_2,...col_k)

NDV(col_1,col_2,...col_k,scale) scale range:【1,10】

Return Type:String
```

`NDV`是一个**模糊**去重函数，返回近似于`COUNT(DISTINCT col)`的结果

它比`COUNT(DISTINCT col)`的组合要快得多，并且使用**恒定大小**的内存



它常用于替代count distinct，通过结合rollup在业务上用于快速计算uv等

## example
```
MySQL > select count(distinct col1) from sample_data;
+---------------------+
| count(distinct col1)|
+---------------------+
| 100000              |
+---------------------+
Fetched 1 row(s) in 20.13s

select cast(ndv(col1) as bigint) as col1 from sample_data;
+----------+
| col1     |
+----------+
| 139017   |
+----------+
Fetched 1 row(s) in 8.91s

```

在一次sql查询中，多次调用ndv，对其速度影响也并不大，这样可以快速得到哪些列的唯一值更少

这比一次sql查询多次调用`count distinct`组合要快得多
```
select cast(ndv(col1) as bigint) as col1, cast(ndv(col2) as bigint) as col2,
cast(ndv(col3) as bigint) as col3, cast(ndv(col4) as bigint) as col4
from sample_data;
+----------+-----------+------------+-----------+
| col1     | col2      | col3       | col4      |
+----------+-----------+------------+-----------+
| 139017   | 282       | 46         | 145636240 |
+----------+-----------+------------+-----------+
Fetched 1 row(s) in 34.97s

select count(distinct col1) from sample_data;
+---------------------+
| count(distinct col1)|
+---------------------+
| 100000              |
+---------------------+
Fetched 1 row(s) in 20.13s

select count(distinct col2) from sample_data;
+----------------------+
| count(distinct col2) |
+----------------------+
| 278                  |
+----------------------+
Fetched 1 row(s) in 20.09s

select count(distinct col3) from sample_data;
+-----------------------+
| count(distinct col3)  |
+-----------------------+
| 46                    |
+-----------------------+
Fetched 1 row(s) in 19.12s

select count(distinct col4) from sample_data;
+----------------------+
| count(distinct col4) |
+----------------------+
| 147135880            |
+----------------------+
Fetched 1 row(s) in 266.95s
```

## keyword
NDV
