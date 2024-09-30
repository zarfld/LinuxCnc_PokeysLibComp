import os
import requests
from datetime import datetime, timedelta

# GitHub repository and token
REPO = "zarfld/LinuxCnc_PokeysLibComp"
TOKEN = os.getenv("GITHUB_TOKEN")

# Labels and keywords
LABELS_KEYWORDS = {
    "good first issue": ["beginner", "easy", "first-time", "simple", "docs"],
    "documentation": ["README", "docs", "documentation"],
    "help wanted": ["help"],
    "bug": ["bug", "error", "issue"],
    "enhancement": ["feature", "enhancement", "improvement"]
}

# Confidence threshold
CONFIDENCE_THRESHOLD = 0.8

def get_issues():
    url = f"https://api.github.com/repos/{REPO}/issues"
    headers = {"Authorization": f"token {TOKEN}"}
    response = requests.get(url, headers=headers)
    response.raise_for_status()
    return response.json()

def analyze_issue(issue):
    title = issue["title"].lower()
    body = issue["body"].lower() if issue["body"] else ""
    for label, keywords in LABELS_KEYWORDS.items():
        for keyword in keywords:
            if keyword in title or keyword in body:
                return label
    return None

def add_label(issue_number, label):
    url = f"https://api.github.com/repos/{REPO}/issues/{issue_number}/labels"
    headers = {"Authorization": f"token {TOKEN}"}
    data = {"labels": [label]}
    response = requests.post(url, headers=headers, json=data)
    response.raise_for_status()

def main():
    issues = get_issues()
    for issue in issues:
        if "pull_request" in issue or issue.get("labels"):
            continue
        label = analyze_issue(issue)
        if label:
            add_label(issue["number"], label)
        else:
            url = f"https://api.github.com/repos/{REPO}/issues/{issue['number']}/comments"
            headers = {"Authorization": f"token {TOKEN}"}
            data = {"body": "This issue could not be confidently labeled. Please review manually."}
            response = requests.post(url, headers=headers, json=data)
            response.raise_for_status()

if __name__ == "__main__":
    main()
