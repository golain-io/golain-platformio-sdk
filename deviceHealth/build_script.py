import os


os.chdir("src/protos")
os.system("cp deviceHealth.proto ../../nanopb/generator")
os.chdir("../../nanopb/generator")
os.system("python3 nanopb_generator.py deviceHealth.proto")
os.remove("deviceHealth.proto")
os.system("cp deviceHealth.pb.c ../../src")
os.system("cp  deviceHealth.pb.h ../../include")
os.chdir("..")
os.system("cp pb.h pb_encode.h pb_decode.h pb_common.h ../include")
os.system("cp pb_encode.c pb_decode.c pb_common.c ../src")
