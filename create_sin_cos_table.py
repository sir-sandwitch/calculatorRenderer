#create lookup tables for sin and cos as a c header file
import math
import numpy as np

# create a lookup table for sin and cos
# the table is 2048 entries long
# the table is for 0 to (1/2)*pi
# the table is using 32 bit floating point numbers

# create the sin table
sin_table = np.zeros(2048)
for i in range(2048):
    sin_table[i] = math.sin(i * math.pi / 4096)

# create the cos table
cos_table = np.zeros(2048)
for i in range(2048):
    cos_table[i] = math.cos(i * math.pi / 4096)

# write the sin and cos tables to a c header file
f = open('sin_cos_table.h', 'w')
f.write('#ifndef SIN_COS_TABLE_H\n')
f.write('#define SIN_COS_TABLE_H\n\n')
f.write('const float sin_table[2048] = {\n')
for i in range(2048):
    f.write('    ' + str(sin_table[i]) + ',\n')
f.write('};\n\n')
f.write('const float cos_table[2048] = {\n')
for i in range(2048):
    f.write('    ' + str(cos_table[i]) + ',\n')
f.write('};\n\n')
f.write('#endif\n')
f.close()
