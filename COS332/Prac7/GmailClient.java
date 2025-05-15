import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.net.Socket;
import javax.net.ssl.SSLSocketFactory;
import java.util.ArrayList;
import java.util.List;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

public class GmailClient {
    private Socket socket;
    private BufferedReader in;
    private PrintWriter out;
    private List<EmailMessage> messages = new ArrayList<>();
    private boolean recentModeEnabled = false;
    private JTable messageTable;
    private JSpinner limitSpinner;
    private JTextField searchField;
    private int limit = 50;

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            try {
                new GmailClient().createAndShowGUI();
            } catch (Exception e) {
                e.printStackTrace();
            }
        });
    }

    private void createAndShowGUI() {
        JFrame frame = new JFrame("Gmail POP3 Client (Recent Mode)");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(1000, 600);

        JPanel loginPanel = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(5, 5, 5, 5);
        gbc.fill = GridBagConstraints.HORIZONTAL;

        gbc.gridx = 0;
        gbc.gridy = 0;
        loginPanel.add(new JLabel("Gmail Address:"), gbc);

        gbc.gridx = 1;
        JTextField usernameField = new JTextField(25);
        loginPanel.add(usernameField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 1;
        loginPanel.add(new JLabel("App Password:"), gbc);

        gbc.gridx = 1;
        JPasswordField passwordField = new JPasswordField(25);
        loginPanel.add(passwordField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 2;
        gbc.gridwidth = 2;
        JCheckBox recentModeCheckbox = new JCheckBox("Use Recent Mode (last 30 days only)", true);
        loginPanel.add(recentModeCheckbox, gbc);

        gbc.gridy = 3;
        JButton connectButton = new JButton("Connect to Gmail");
        loginPanel.add(connectButton, gbc);

        DefaultTableModel tableModel = new DefaultTableModel(
            new Object[]{"Delete", "From", "Subject", "Size", "Date"}, 0) {
            @Override
            public Class<?> getColumnClass(int columnIndex) {
                return columnIndex == 0 ? Boolean.class : 
                       columnIndex == 3 ? Integer.class : 
                       columnIndex == 4 ? Date.class : 
                       String.class;
            }

            @Override
            public Object getValueAt(int row, int column) {
                if (column == 4) {
                    return messages.get(row).dateObj;
                }
                return super.getValueAt(row, column);
            }
        };

        messageTable = new JTable(tableModel);
        messageTable.setAutoCreateRowSorter(true);
        JScrollPane tableScrollPane = new JScrollPane(messageTable);

        JLabel statusBar = new JLabel("Ready to connect...");
        statusBar.setBorder(BorderFactory.createEtchedBorder());

        JButton refreshButton = new JButton("Refresh");
        JButton deleteButton = new JButton("Delete Selected");
        JPanel buttonPanel = new JPanel();
        buttonPanel.add(refreshButton);
        buttonPanel.add(deleteButton);

        JPanel limitPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JLabel limitLabel = new JLabel("Show max messages:");
        limitSpinner = new JSpinner(new SpinnerNumberModel(50, 10, 1000, 10));
        limitPanel.add(limitLabel);
        limitPanel.add(limitSpinner);

        JPanel searchPanel = new JPanel(new BorderLayout(5, 5));
        searchField = new JTextField();
        JButton searchButton = new JButton("Search");
        searchPanel.add(new JLabel("Search (Subject/From):"), BorderLayout.WEST);
        searchPanel.add(searchField, BorderLayout.CENTER);
        searchPanel.add(searchButton, BorderLayout.EAST);

        frame.setLayout(new BorderLayout());
        frame.add(loginPanel, BorderLayout.NORTH);


        JPanel centerPanel = new JPanel(new BorderLayout());
        centerPanel.add(tableScrollPane, BorderLayout.CENTER);


        JPanel controlsPanel = new JPanel(new BorderLayout());
        controlsPanel.add(limitPanel, BorderLayout.WEST);
        controlsPanel.add(searchPanel, BorderLayout.CENTER);
        centerPanel.add(controlsPanel, BorderLayout.NORTH);

        frame.add(centerPanel, BorderLayout.CENTER);

        JPanel southPanel = new JPanel(new BorderLayout());
        southPanel.add(buttonPanel, BorderLayout.NORTH);
        southPanel.add(statusBar, BorderLayout.SOUTH);
        frame.add(southPanel, BorderLayout.SOUTH);

        connectButton.addActionListener(e -> {
            String username = usernameField.getText().trim();
            String password = new String(passwordField.getPassword()).trim();
            recentModeEnabled = recentModeCheckbox.isSelected();

            if (username.isEmpty() || password.isEmpty()) {
                statusBar.setText("Please enter both email and password");
                return;
            }

            try {
                connectAndAuthenticate(username, password);
                listMessages();
                updateTable(tableModel);
                statusBar.setText("Connected successfully. " + messages.size() + 
                    " messages found" + (recentModeEnabled ? " (Recent Mode)" : ""));
            } catch (Exception ex) {
                statusBar.setText("Error: " + ex.getMessage());
                JOptionPane.showMessageDialog(frame, 
                    "Connection failed:\n" + ex.getMessage(), 
                    "Error", JOptionPane.ERROR_MESSAGE);
                ex.printStackTrace();
            }
        });

        refreshButton.addActionListener(e -> {
            try {
                listMessages();
                updateTable(tableModel);
                statusBar.setText("Refreshed. " + messages.size() + 
                    " messages found" + (recentModeEnabled ? " (Recent Mode)" : ""));
            } catch (Exception ex) {
                statusBar.setText("Error refreshing: " + ex.getMessage());
                JOptionPane.showMessageDialog(frame, 
                    "Refresh failed:\n" + ex.getMessage(), 
                    "Error", JOptionPane.ERROR_MESSAGE);
            }
        });

        deleteButton.addActionListener(e -> {
            try {
                int deleteCount = 0;
                for (int i = 0; i < tableModel.getRowCount(); i++) {
                    if ((Boolean)tableModel.getValueAt(i, 0)) {
                        deleteMessage(messages.get(i).number);
                        deleteCount++;
                    }
                }
                listMessages();
                updateTable(tableModel);
                statusBar.setText("Deleted " + deleteCount + " messages. " + 
                    messages.size() + " remaining");
                JOptionPane.showMessageDialog(frame, 
                    "Deleted " + deleteCount + " messages successfully.", 
                    "Success", JOptionPane.INFORMATION_MESSAGE);
            } catch (Exception ex) {
                statusBar.setText("Error deleting: " + ex.getMessage());
                JOptionPane.showMessageDialog(frame, 
                    "Delete failed:\n" + ex.getMessage(), 
                    "Error", JOptionPane.ERROR_MESSAGE);
            }
        });

        searchButton.addActionListener(e -> {
            updateTable(tableModel);
        });

        searchField.addActionListener(e -> {
            updateTable(tableModel);
        });

        limitSpinner.addChangeListener(e -> {
            updateTable(tableModel);
        });

        frame.setVisible(true);
    }

    private void connectAndAuthenticate(String username, String password) throws IOException {
        try {
            SSLSocketFactory sslSocketFactory = (SSLSocketFactory)SSLSocketFactory.getDefault();
            socket = sslSocketFactory.createSocket("pop.gmail.com", 995);
            socket.setSoTimeout(60000);
            
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            out = new PrintWriter(socket.getOutputStream(), true);

            String response = in.readLine();
            if (response == null || !response.startsWith("+OK")) {
                throw new IOException("Server error: " + response);
            }

            sendCommand("USER " + (recentModeEnabled ? "recent:" + username : username));
            sendCommand("PASS " + password);

            sendCommand("STAT");
            
        } catch (IOException e) {
            disconnect();
            throw new IOException("Connection failed: " + e.getMessage());
        }
    }

    private void listMessages() throws IOException {
        messages.clear();

        String statResponse = sendCommand("STAT");
        String[] statParts = statResponse.split(" ");
        int messageCount = Integer.parseInt(statParts[1]);
        
        if (messageCount == 0) return;

        sendCommand("LIST");
        List<String> listLines = readMultilineResponse();
        int count = 0;
        for (String response : listLines) {
            String[] parts = response.split(" ");
            if (parts.length >= 2) {
                try {
                    int msgNum = Integer.parseInt(parts[0]);
                    int size = Integer.parseInt(parts[1]);
                    String headers = getHeaders(msgNum);
                    String from = extractHeader(headers, "From");
                    String subject = extractHeader(headers, "Subject");
                    String date = extractHeader(headers, "Date");
                    messages.add(new EmailMessage(msgNum, from, subject, size, date));
                } catch (NumberFormatException e) {
                    System.err.println("Skipping malformed LIST response: " + response);
                }
            }
            if(++count >= limit)
            {
              break;
            }
        }
    }

    private List<String> readMultilineResponse() throws IOException {
        List<String> lines = new ArrayList<>();
        String line;
        while ((line = in.readLine()) != null) {
            if (line.equals(".")) break;
            if (line.startsWith("..")) line = line.substring(1);
            lines.add(line);
        }
        return lines;
    }

    private String getHeaders(int msgNum) throws IOException {
        sendCommand("TOP " + msgNum + " 0");
        StringBuilder headers = new StringBuilder();
        List<String> headerLines = readMultilineResponse();
        for (String line : headerLines) {
            headers.append(line).append("\n");
        }
        return headers.toString();
    }

    private String extractHeader(String headers, String headerName) {
        String[] lines = headers.split("\n");
        for (String line : lines) {
            if (line.toLowerCase().startsWith(headerName.toLowerCase() + ":")) {
                return line.substring(line.indexOf(":") + 1).trim();
            }
        }
        return "[No " + headerName + "]";
    }

    private void deleteMessage(int msgNum) throws IOException {
        sendCommand("DELE " + msgNum);
    }

    private String sendCommand(String command) throws IOException {
        System.out.println("Sending command: " + command);
        out.println(command);
        String response = in.readLine();
        if (response == null) {
            throw new IOException("No response from server");
        }
        if (response.startsWith("-ERR")) {
            throw new IOException("Server error: " + response.substring(5).trim());
        }
        return response;
    }

    private void disconnect() {
        try {
            if (out != null) {
                sendCommand("QUIT");
            }
        } catch (Exception e) {
            System.out.println("Error during disconnect: " + e.getMessage());
        }
        
        try { if (in != null) in.close(); } catch (IOException e) {}
        try { if (out != null) out.close(); } catch (Exception e) {}
        try { if (socket != null) socket.close(); } catch (IOException e) {}
    }

    private void updateTable(DefaultTableModel tableModel) {
        tableModel.setRowCount(0);
        limit = (Integer)limitSpinner.getValue();
        String searchText = searchField.getText().toLowerCase();
        int count = 0;
        
        for (EmailMessage msg : messages) {
            if (!searchText.isEmpty() && 
                !msg.subject.toLowerCase().contains(searchText) && 
                !msg.from.toLowerCase().contains(searchText)) {
                continue;
            }
            if (count >= limit) {
                break;
            }
            
            tableModel.addRow(new Object[]{
                false, 
                msg.from, 
                msg.subject, 
                msg.size, 
                msg.dateObj
            });
            count++;
        }
        messageTable.getColumnModel().getColumn(4).setCellRenderer(new DefaultTableCellRenderer() {
            SimpleDateFormat fmt = new SimpleDateFormat("EEE, d MMM yyyy HH:mm:ss");
            
            @Override
            public Component getTableCellRendererComponent(JTable table, Object value,
                    boolean isSelected, boolean hasFocus, int row, int column) {
                if (value instanceof Date) {
                    value = fmt.format((Date)value);
                }
                return super.getTableCellRendererComponent(table, value, isSelected, 
                        hasFocus, row, column);
            }
        });

        messageTable.getColumnModel().getColumn(3).setCellRenderer(new DefaultTableCellRenderer() {
            @Override
            public Component getTableCellRendererComponent(JTable table, Object value,
                    boolean isSelected, boolean hasFocus, int row, int column) {
                if (value instanceof Integer) {
                    value = formatSize((Integer)value);
                }
                return super.getTableCellRendererComponent(table, value, isSelected, 
                        hasFocus, row, column);
            }
        });
    }

    private String formatSize(int size) {
        if (size < 1024) return size + " B";
        if (size < 1024 * 1024) return (size / 1024) + " KB";
        return String.format("%.1f MB", size / (1024.0 * 1024.0));
    }

    private static class EmailMessage {
        int number;
        String from;
        String subject;
        int size;
        String date;
        Date dateObj;

        public EmailMessage(int number, String from, String subject, int size, String date) {
            this.number = number;
            this.from = from;
            this.subject = subject;
            this.size = size;
            this.date = date;
            this.dateObj = parseDate(date);
        }

        private Date parseDate(String dateStr) {
            if (dateStr == null || dateStr.equals("[No Date]")) {
                return new Date(0);
            }
            SimpleDateFormat[] formats = {
                new SimpleDateFormat("EEE, d MMM yyyy HH:mm:ss Z", Locale.ENGLISH),
                new SimpleDateFormat("d MMM yyyy HH:mm:ss Z", Locale.ENGLISH),
                new SimpleDateFormat("EEE, d MMM yyyy HH:mm:ss", Locale.ENGLISH),
                new SimpleDateFormat("d MMM yyyy HH:mm:ss", Locale.ENGLISH)
            };
            
            for (SimpleDateFormat format : formats) {
                try {
                    return format.parse(dateStr);
                } catch (ParseException e) {
                }
            }
            
            return new Date(0);
        }
    }
}