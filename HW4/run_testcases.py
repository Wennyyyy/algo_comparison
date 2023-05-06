import argparse
import logging
import re
import os
import subprocess
import io
import pandas as pd

args = None
logger = None


def arg_parse():
    """
    解析參數設定並回傳解析結果。

    Returns:
        argparse.Namespace: 解析後的參數設定。
    """

    parser = argparse.ArgumentParser(description='twstock test script')
    # 設定參數選項
    parser.add_argument('--version', action='version', version='%(prog)s 0.1')
    parser.add_argument('-v', '--verbose', action='count', default=0)
    parser.add_argument('-l', '--log', dest='log', type=str,
                        metavar='LOG_FILE', default="run_testcases.log", help='log file name')

    # 解析參數
    return parser.parse_args()
# End of arg_parse


def get_logger():
    """
    用來設定 logging。

    這個程式會修改全域變數logger，使他成為logging的logger，
    並會使log輸出到終端機並導到twstock_test.log

    Returns:
        logging.Logger
    """

    # 設定日誌格式
    formatter = logging.Formatter('[%(levelname)s] %(message)s')

    # 設定輸出到 console 的 handler
    stream_handler = logging.StreamHandler()
    stream_handler.setFormatter(formatter)

    # 設定輸出到檔案的 handler
    file_handler = logging.FileHandler(args.log, 'w', 'utf-8')
    file_handler.setFormatter(formatter)

    # 設定 logger，並加入 handlers
    logger = logging.getLogger()
    if args.verbose:
        logger.setLevel(logging.DEBUG)
    else:
        logger.setLevel(logging.INFO)
    logger.addHandler(stream_handler)
    logger.addHandler(file_handler)

    return logger
# End of log_setting


if __name__ == '__main__':
    args = arg_parse()  # 命令參數解析
    logger = get_logger()  # 取得logger
    testcase_path = 'testcase'
    # log_path = 'log'

    # 檢查資料夾是否存在
    if not os.path.exists(testcase_path):
        logger.critical('There is no ' + testcase_path)
        exit()

    # if not os.path.exists(log_path):
    #     os.mkdir(log_path)

    # Build exe
    os.system("powershell.exe \"make\"")

    # 初始化資料
    scores = pd.DataFrame(
        {},
        index=[],
    )

    # 取得資料夾下的所有檔案列表
    for f in os.listdir(testcase_path):
        if (re.match("testcase", f)):
            match = re.match("testcase(\d+)-(\d+)$", f)
            n = int(match.group(1))
            if n > 9:
                continue
            idx = match.group(2)
            repeat_times = 8
            if (n > 11):
                repeat_times = 4
            if (n > 12):
                repeat_times = 1
            for i in range(repeat_times):
                no_result = True
                cmd = ".\\algo4_bruteforce.exe {}\\{}".format(testcase_path, f)
                logger.info(
                    "========================================================")
                logger.info("Start {}th {}".format(i, f))
                logger.debug(cmd)
                p = subprocess.Popen(cmd, stdout=subprocess.PIPE, shell=True)

                # 獲取標準輸出
                stdout, stderr = p.communicate()
                stdout = stdout.decode("utf8")
                stdout = stdout.replace('\r', '')
                logger.debug("Output:\n" + stdout)
                for line in io.StringIO(stdout):
                    match = re.search('Time:\s+(\d+)(\.\d+)?\s+s', line)

                    if match:
                        no_result = False
                        if match.group(2) is not None:
                            time = float(match.group(1)+match.group(2))
                        else:
                            time = float(match.group(1))

                        logger.info(
                            "{}th {} cost {} sec".format(i, f, time))
                        if i == 0:
                            if idx in scores.keys():
                                if 'n = {}'.format(n) in scores.index:
                                    scores[idx]['n = {}'.format(n)] = time
                                else:
                                    new_scores = pd.DataFrame({
                                        idx: time
                                    }, index=['n = {}'.format(n)])
                                    scores = pd.concat([scores, new_scores])
                                    scores = scores.sort_index(
                                        key=lambda index: [int(x[1]) for x in index.str.split(" = ")])
                            else:
                                scores[idx] = pd.Series(
                                    [time], index=['n = {}'.format(n)])
                        else:
                            scores[idx]['n = {}'.format(n)] = (
                                scores[idx]['n = {}'.format(n)]*i+time)/(i+1)

                        logger.info("Temp result:\n" + str(scores) + "\n\n")

                        # 將DataFrame對象保存到Excel文件中
                        scores.to_excel('Brute Force.xlsx')
                        break
                if no_result:
                    logger.error("{}th {} fail. Output:{}".format(
                        i, f, stdout.decode("utf8")))
                    break

    scores['Avg'] = scores.mean(axis=1)
    logger.info("Temp result:\n" + str(scores) + "\n")

    # 將DataFrame對象保存到Excel文件中
    scores.to_excel('Brute Force.xlsx')
