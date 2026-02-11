import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;

public class MiniPhoneBookGUI extends JFrame {
    private JTextField nameField, phoneField, searchField;
    private DefaultListModel<String> listModel;
    private JList<String> contactList;
    private ArrayList<Contact> contacts = new ArrayList<>();

    public MiniPhoneBookGUI() {
        setTitle("Mini Phone Book");
        setSize(500, 500);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        JPanel topPanel = new JPanel(new GridLayout(3, 2));
        nameField = new JTextField();
        phoneField = new JTextField();
        searchField = new JTextField();

        topPanel.add(new JLabel("Name:"));
        topPanel.add(nameField);
        topPanel.add(new JLabel("Phone:"));
        topPanel.add(phoneField);
        topPanel.add(new JLabel("Search:"));
        topPanel.add(searchField);

        add(topPanel, BorderLayout.NORTH);

        listModel = new DefaultListModel<>();
        contactList = new JList<>(listModel);
        add(new JScrollPane(contactList), BorderLayout.CENTER);

        JPanel buttonPanel = new JPanel();
        JButton addButton = new JButton("Add");
        JButton updateButton = new JButton("Update");
        JButton deleteButton = new JButton("Delete");
        JButton saveButton = new JButton("Save");
        JButton loadButton = new JButton("Load");

        buttonPanel.add(addButton);
        buttonPanel.add(updateButton);
        buttonPanel.add(deleteButton);
        buttonPanel.add(saveButton);
        buttonPanel.add(loadButton);

        add(buttonPanel, BorderLayout.SOUTH);

        addButton.addActionListener(e -> addContact());
        updateButton.addActionListener(e -> updateContact());
        deleteButton.addActionListener(e -> deleteContact());
        saveButton.addActionListener(e -> saveContacts());
        loadButton.addActionListener(e -> loadContacts());

        searchField.getDocument().addDocumentListener(new javax.swing.event.DocumentListener() {
            public void changedUpdate(javax.swing.event.DocumentEvent e) { searchContacts(); }
            public void removeUpdate(javax.swing.event.DocumentEvent e) { searchContacts(); }
            public void insertUpdate(javax.swing.event.DocumentEvent e) { searchContacts(); }
        });

        setVisible(true);
    }

    private void addContact() {
        String name = nameField.getText().trim();
        String phone = phoneField.getText().trim();
        if (!name.isEmpty() && !phone.isEmpty()) {
            contacts.add(new Contact(name, phone));
            refreshList();
            nameField.setText("");
            phoneField.setText("");
        }
    }

    private void updateContact() {
        int index = contactList.getSelectedIndex();
        if (index >= 0 && index < contacts.size()) {
            contacts.set(index, new Contact(nameField.getText(), phoneField.getText()));
            refreshList();
        }
    }

    private void deleteContact() {
        int index = contactList.getSelectedIndex();
        if (index >= 0 && index < contacts.size()) {
            contacts.remove(index);
            refreshList();
        }
    }

    private void saveContacts() {
        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("contacts.dat"))) {
            oos.writeObject(contacts);
        } catch (IOException ex) {
            JOptionPane.showMessageDialog(this, "Error saving contacts");
        }
    }

    private void loadContacts() {
        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream("contacts.dat"))) {
            contacts = (ArrayList<Contact>) ois.readObject();
            refreshList();
        } catch (IOException | ClassNotFoundException ex) {
            JOptionPane.showMessageDialog(this, "Error loading contacts");
        }
    }

    private void searchContacts() {
        String query = searchField.getText().toLowerCase();
        listModel.clear();
        contacts.stream()
                .filter(c -> c.name.toLowerCase().startsWith(query))
                .sorted(Comparator.comparing(c -> c.name.toLowerCase()))
                .forEach(c -> listModel.addElement(c.name + " - " + c.phone));
    }

    private void refreshList() {
        listModel.clear();
        contacts.stream()
                .sorted(Comparator.comparing(c -> c.name.toLowerCase()))
                .forEach(c -> listModel.addElement(c.name + " - " + c.phone));
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(MiniPhoneBookGUI::new);
    }

    static class Contact implements Serializable {
        String name, phone;
        Contact(String name, String phone) {
            this.name = name;
            this.phone = phone;
        }
    }
}