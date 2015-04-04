import os

data_dir = "D:\\Dev\\bio\\data\\"
app = "app.exe"
parameters = [0.8, 1, 1.2, 1.5, 1.8, 2, 2.2, 2.5, 2.8, 3, 3.2, 3.5]

cancer_dirs = [os.path.join(data_dir, name) for name in os.listdir(data_dir) if os.path.isdir(os.path.join(data_dir, name))]

for cancer_dir in cancer_dirs:
    test_dirs = [os.path.join(cancer_dir, name) for name in os.listdir(cancer_dir) if os.path.isdir(os.path.join(cancer_dir, name))]
    for test_dir in test_dirs:
        for param in parameters:
            out_dir = os.path.join(test_dir, "test_" + str(param).replace(".", "_"))
            if not os.path.exists(out_dir):
                os.makedirs(out_dir)
            cmd = " ".join([app, "generate", os.path.join(test_dir, "train.csv"), os.path.join(test_dir, "test.csv"), os.path.join(out_dir, "metrics.csv"),
                           ">", os.path.join(out_dir, "log.txt")])
            print "cmd: " + cmd
            os.system(cmd)