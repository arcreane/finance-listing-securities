import yfinance as yf
import pandas as pd
import zipfile
import os

def fetch_data(ticker="KO", period="1y"):
    print(f"Fetching data for {ticker}...")
    data = yf.download(ticker, period=period)
    # Check if data is multi-index (common in recent yfinance updates)
    if isinstance(data.columns, pd.MultiIndex):
        data.columns = data.columns.get_level_values(0)
    return data

def save_to_sql(df, filename="coca_cola_data.sql"):
    print(f"Converting data to {filename}...")
    with open(filename, "w", encoding="utf-8") as f:
        f.write("CREATE TABLE IF NOT EXISTS coca_cola_prices (\n")
        f.write("    Date DATE PRIMARY KEY,\n")
        f.write("    Open FLOAT,\n")
        f.write("    High FLOAT,\n")
        f.write("    Low FLOAT,\n")
        f.write("    Close FLOAT,\n")
        f.write("    Adj_Close FLOAT,\n")
        f.write("    Volume BIGINT\n")
        f.write(");\n\n")
        
        # Recent yfinance versions might use different column names or structures
        print(f"Available columns: {list(df.columns)}")
        
        for index, row in df.iterrows():
            # Map columns safely using .get() if row is a Series, or just indexing
            # Note: row here is a pandas Series
            open_val = row['Open'] if 'Open' in row else row.get('open', 0)
            high_val = row['High'] if 'High' in row else row.get('high', 0)
            low_val = row['Low'] if 'Low' in row else row.get('low', 0)
            close_val = row['Close'] if 'Close' in row else row.get('close', 0)
            adj_close_val = row['Adj Close'] if 'Adj Close' in row else row.get('adj close', close_val)
            volume_val = row['Volume'] if 'Volume' in row else row.get('volume', 0)

            date_str = index.strftime('%Y-%m-%d')
            values = f"('{date_str}', {open_val}, {high_val}, {low_val}, {close_val}, {adj_close_val}, {volume_val})"
            f.write(f"REPLACE INTO coca_cola_prices (Date, Open, High, Low, Close, Adj_Close, Volume) VALUES {values};\n")
    print("SQL file created.")

def zip_file(source_file, zip_name="coca_cola_data.zip"):
    print(f"Zipping {source_file} into {zip_name}...")
    with zipfile.ZipFile(zip_name, 'w', zipfile.ZIP_DEFLATED) as zipf:
        zipf.write(source_file, os.path.basename(source_file))
    print("Zip file created.")

if __name__ == "__main__":
    ticker_symbol = "KO"
    sql_filename = "coca_cola_data.sql"
    zip_filename = "coca_cola_data.zip"
    
    try:
        df = fetch_data(ticker_symbol)
        if df.empty:
            print("No data found.")
        else:
            save_to_sql(df, sql_filename)
            zip_file(sql_filename, zip_filename)
            print("Success!")
    except Exception as e:
        print(f"An error occurred: {e}")
