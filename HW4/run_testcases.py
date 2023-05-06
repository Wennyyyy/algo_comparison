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
                        metavar='FILE', default="run_testcases.log", help='log file name')
    parser.add_argument('--input_dir', dest='input_dir', type=str,
                        metavar='DIR', default="data", help='input dir')
    parser.add_argument('--result', dest='result', type=str,
                        metavar='FILE', default="BnB.xlsx", help='excel file name')
    parser.add_argument('--exe', dest='exe', type=str,
                        metavar='FILE', default="hw4_bnb.exe", help='exe file name')
    parser.add_argument('--max_n', dest='max_n', type=int,
                        metavar='NUM', default="100", help='max n to execute')

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
    input_path = args.input_dir
    report_path = 'Report'

    # 檢查資料夾是否存在
    if not os.path.exists(input_path):
        logger.critical('There is no ' + input_path)
        exit()

    if not os.path.exists(report_path):
        logger.critical('There is no ' + report_path)
        exit()
    report_file = os.path.join(report_path, args.result)

    # Build exe
    os.system("make EXEC={}".format(args.exe))

    # 初始化資料表格
    scores = pd.DataFrame(
        {},
        index=[],
    )

    # 取得資料夾下的所有檔案列表
    for f in os.listdir(input_path):
        match = re.match("testcase(\d+)-(\d+).txt$", f)
        if (match):
            n = int(match.group(1))
            if n > args.max_n:
                continue
            idx = 'n = {}'.format(n)
            key = int(match.group(2))
            repeat_times = 5

            logger.info(f)
            for i in range(repeat_times):
                no_result = True
                input_file = os.path.join(input_path, f)
                exe_file = os.path.join('.', args.exe)
                cmd = "{} < {} ".format(exe_file, input_file)
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
                    match_int = re.search('Time:\s+(\d+)$', line)
                    match_float = re.search('Time:\s+(\d+\.\d+)$', line)
                    match_exp = re.search(
                        'Time:\s+(\d*\.?\d+([eE][-+]?\d+)?)$', line)

                    if match_int or match_float or match_exp:
                        no_result = False
                        if match_int:
                            time = float(match_int.group(1))
                        elif match_float:
                            time = float(match_float.group(1))
                        else:
                            time = float(match_exp.group(1))

                        logger.info(
                            "{}th {} cost {} sec".format(i, f, time))
                        if i == 0:
                            if key in scores.keys():
                                if idx in scores.index:
                                    scores[key][idx] = time
                                else:
                                    new_scores = pd.DataFrame({
                                        key: time
                                    }, index=[idx])

                                    scores = pd.concat([scores, new_scores])

                                    # file executed in
                                    scores = scores.sort_index(
                                        key=lambda index: [int(x[1]) for x in index.str.split(" = ")])
                            else:
                                scores[key] = pd.Series(
                                    [time], index=[idx])
                        else:
                            # calculate average time for each testcase repeat_times
                            scores[key][idx] = (
                                scores[key][idx]*i+time)/(i+1)

                        logger.info("Temp result:\n" + str(scores) + "\n\n")

                        # 將DataFrame對象保存到Excel文件中
                        scores.to_excel(report_file)
                        break
                if no_result:
                    logger.error("{}th {} fail. Output:{}".format(
                        i, f, stdout.decode("utf8")))
                    break

    scores['Avg'] = scores.mean(axis=1)
    logger.info("Temp result:\n" + str(scores) + "\n")

    # 將DataFrame對象保存到Excel文件中
    scores.to_excel(report_file)
